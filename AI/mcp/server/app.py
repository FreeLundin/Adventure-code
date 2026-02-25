from typing import List, Dict, Any, Optional
from datetime import datetime
import os
import json

from fastapi import FastAPI, HTTPException, Header, Depends, Request
from fastapi.security.api_key import APIKeyHeader
from fastapi.security import HTTPBearer, HTTPAuthorizationCredentials
from jose import JWTError, jwt
from sqlmodel import SQLModel, Field, create_engine, Session, select
from prometheus_client import Counter, generate_latest, CONTENT_TYPE_LATEST
import requests
import jsonschema

# authentication modes: "apikey" (default) or "jwt"
AUTH_MODE = os.getenv("AUTH_MODE", "apikey")
JWT_SECRET = os.getenv("JWT_SECRET", "supersecret")
JWT_ALGORITHM = os.getenv("JWT_ALGORITHM", "HS256")

# Prometheus metrics
reg_counter = Counter("mcp_register_total", "Number of model registrations")
promote_counter = Counter("mcp_promote_total", "Number of promotions")
metric_ingest_counter = Counter("mcp_metrics_total", "Metrics posted")

# load schema for validation
with open(os.path.join(os.path.dirname(__file__), "..", "model_card_schema.json")) as f:
    MODEL_CARD_SCHEMA = json.load(f)

# ----- database setup -----

DATABASE_URL = os.getenv("DATABASE_URL", "sqlite:///./mcp.db")
engine = create_engine(DATABASE_URL, echo=False)


def init_db():
    SQLModel.metadata.create_all(engine)


# ----- SQLModel models -----

class ModelCard(SQLModel, table=True):
    name: str = Field(primary_key=True)
    version: str = Field(primary_key=True)
    description: Optional[str] = None
    owner: Optional[str] = None
    license: Optional[str] = None
    tags: Optional[str] = None  # comma-separated
    created_at: datetime = Field(default_factory=datetime.utcnow)
    updated_at: datetime = Field(default_factory=datetime.utcnow)
    provenance: Optional[str] = None  # JSON string
    # use alias to avoid collision with SQLAlchemy "metadata" attr
    metadata_: Optional[str] = Field(default=None, alias="metadata")  # JSON string


class Promotion(SQLModel, table=True):
    name: str = Field(primary_key=True)
    version: str = Field(primary_key=True)
    target: str
    canary_percent: Optional[int] = None
    updated_at: datetime = Field(default_factory=datetime.utcnow)


class MetricEntry(SQLModel, table=True):
    id: Optional[int] = Field(primary_key=True)
    model_name: str
    model_version: str
    metrics: str
    timestamp: datetime = Field(default_factory=datetime.utcnow)


# ----- Pydantic request models (keep for compatibility) -----

class RegisterModelRequest(SQLModel):
    name: str
    version: str
    model_url: str
    model_card_url: str
    checksum: Optional[str] = None
    metadata: Optional[Dict[str, Any]] = None


class PromoteModelRequest(SQLModel):
    name: str
    version: str
    target: str
    canary_percent: Optional[int] = None


class RollbackRequest(SQLModel):
    name: str
    to_version: str


class MetricsRequest(SQLModel):
    model_name: str
    model_version: str
    metrics: Dict[str, Any]


# ----- security dependency -----

API_KEY_NAME = "X-API-Key"
api_key_header = APIKeyHeader(name=API_KEY_NAME, auto_error=False)
bearer_scheme = HTTPBearer(auto_error=False)


def get_credentials(
    api_key: str = Depends(api_key_header),
    bearer: HTTPAuthorizationCredentials = Depends(bearer_scheme),
):
    """Return a dict with identity info for either api-key or JWT."""
    if AUTH_MODE == "apikey":
        expected = os.getenv("API_KEY", "secret")
        if api_key != expected:
            raise HTTPException(status_code=401, detail="Invalid API key")
        return {"method": "apikey"}
    else:
        # JWT validation
        if not bearer or bearer.scheme.lower() != "bearer":
            raise HTTPException(status_code=401, detail="Missing JWT bearer token")
        try:
            payload = jwt.decode(bearer.credentials, JWT_SECRET, algorithms=[JWT_ALGORITHM])
        except JWTError:
            raise HTTPException(status_code=401, detail="Invalid JWT token")
        # payload may contain tenant/roles
        return {"method": "jwt", "claims": payload}


def check_tenant(name: str, creds: dict = Depends(get_credentials)):
    """Raise 403 if JWT tenant doesn't match model owner."""
    if creds.get("method") == "jwt":
        tenant = creds.get("claims", {}).get("tenant")
        if tenant:
            # look up model owner
            with Session(engine) as session:
                stmt = select(ModelCard).where(ModelCard.name == name)
                cards = session.exec(stmt).all()
                for c in cards:
                    if c.owner and c.owner != tenant:
                        raise HTTPException(status_code=403, detail="Forbidden")
    return creds


# ----- application -----

app = FastAPI(title="Model Control Plane (MCP) API")

@app.get("/prometheus")
def prometheus_metrics():
    return generate_latest(), 200, {"Content-Type": CONTENT_TYPE_LATEST}

@app.on_event("startup")
def on_startup():
    init_db()


@app.post("/models/register", response_model=ModelCard, dependencies=[Depends(get_credentials)])
def register_model(req: RegisterModelRequest, creds: dict = Depends(get_credentials)):
    # fetch & validate model card JSON
    try:
        resp = requests.get(req.model_card_url, timeout=5)
        resp.raise_for_status()
        card_json = resp.json()
        if card_json.get("name") != req.name or card_json.get("version") != req.version:
            raise HTTPException(status_code=400, detail="model_card name/version mismatch")
        # validate schema
        jsonschema.validate(card_json, MODEL_CARD_SCHEMA)
    except HTTPException:
        raise
    except jsonschema.ValidationError as ve:
        raise HTTPException(status_code=400, detail=f"model card schema error: {ve.message}")
    except Exception as ex:
        print(f"warning: could not fetch/validate model_card: {ex}")

    with Session(engine) as session:
        existing = session.get(ModelCard, (req.name, req.version))
        if existing:
            raise HTTPException(status_code=409, detail="version already exists")
        card = ModelCard(
            name=req.name,
            version=req.version,
            metadata_=json.dumps(req.metadata) if req.metadata else None,
        )
        session.add(card)
        session.commit()
        session.refresh(card)
        reg_counter.inc()
        return card


@app.get("/models", response_model=List[ModelCard], dependencies=[Depends(get_credentials)])
def list_models(name: Optional[str] = None, tag: Optional[str] = None, creds: dict = Depends(get_credentials)):
    # ACL: if JWT with tenant claim, filter by owner
    tenant = creds.get("claims", {}).get("tenant") if creds.get("method") == "jwt" else None
    with Session(engine) as session:
        stmt = select(ModelCard)
        if name:
            stmt = stmt.where(ModelCard.name == name)
        if tag:
            stmt = stmt.where(ModelCard.tags.contains(tag))
        if tenant:
            stmt = stmt.where(ModelCard.owner == tenant)
        results = session.exec(stmt).all()
        # deserialize metadata/provenance/tags
        for r in results:
            if r.metadata_:
                r.metadata_ = json.loads(r.metadata_)
            if r.provenance:
                r.provenance = json.loads(r.provenance)
            if r.tags:
                r.tags = r.tags.split(",") if r.tags else []
        return results


@app.get("/models/search", response_model=List[ModelCard], dependencies=[Depends(get_credentials)])
def search_models(query: str, creds: dict = Depends(get_credentials)):
    tenant = creds.get("claims", {}).get("tenant") if creds.get("method") == "jwt" else None
    with Session(engine) as session:
        stmt = select(ModelCard).where(
            (ModelCard.name.contains(query)) | (ModelCard.tags.contains(query))
        )
        if tenant:
            stmt = stmt.where(ModelCard.owner == tenant)
        results = session.exec(stmt).all()
        for r in results:
            if r.metadata_:
                r.metadata_ = json.loads(r.metadata_)
            if r.provenance:
                r.provenance = json.loads(r.provenance)
            if r.tags:
                r.tags = r.tags.split(",")
        return results


# metrics and promotions need to be defined before the catch‑all /models/{name} path
@app.get("/models/metrics", dependencies=[Depends(get_credentials)])
def query_metrics(model_name: Optional[str] = None, model_version: Optional[str] = None):
    with Session(engine) as session:
        stmt = select(MetricEntry)
        if model_name:
            stmt = stmt.where(MetricEntry.model_name == model_name)
        if model_version:
            stmt = stmt.where(MetricEntry.model_version == model_version)
        entries = session.exec(stmt).all()
        return [
            {
                "model_name": e.model_name,
                "model_version": e.model_version,
                "metrics": json.loads(e.metrics),
                "timestamp": e.timestamp,
            }
            for e in entries
        ]

@app.get("/models/{name}/promotions", dependencies=[Depends(get_credentials)])
def get_promotions(name: str):
    with Session(engine) as session:
        stmt = select(Promotion).where(Promotion.name == name)
        proms = session.exec(stmt).all()
        return [
            {"version": p.version, "target": p.target, "canary_percent": p.canary_percent}
            for p in proms
        ]

@app.delete("/models/{name}/promotions", status_code=204, dependencies=[Depends(get_credentials)])
def clear_promotions(name: str):
    with Session(engine) as session:
        stmt = select(Promotion).where(Promotion.name == name)
        proms = session.exec(stmt).all()
        for p in proms:
            session.delete(p)
        session.commit()
    return

@app.get("/models/{name}", response_model=List[ModelCard], dependencies=[Depends(get_credentials)])
def get_model_versions(name: str):
    with Session(engine) as session:
        stmt = select(ModelCard).where(ModelCard.name == name)
        results = session.exec(stmt).all()
        if not results:
            raise HTTPException(status_code=404, detail="Model not found")
        for r in results:
            if r.metadata_:
                r.metadata_ = json.loads(r.metadata_)
            if r.provenance:
                r.provenance = json.loads(r.provenance)
            if r.tags:
                r.tags = r.tags.split(",")
        return results


@app.delete("/models/{name}/versions/{version}", status_code=204, dependencies=[Depends(get_credentials)])
def delete_model_version(name: str, version: str):
    with Session(engine) as session:
        card = session.get(ModelCard, (name, version))
        if not card:
            raise HTTPException(status_code=404, detail="Version not found")
        session.delete(card)
        session.commit()
    return


@app.post("/models/promote", dependencies=[Depends(get_credentials)])
def promote_model(req: PromoteModelRequest):
    with Session(engine) as session:
        promo = Promotion(
            name=req.name,
            version=req.version,
            target=req.target,
            canary_percent=req.canary_percent,
        )
        session.add(promo)
        session.commit()
        session.refresh(promo)
    promote_counter.inc()
    return {"status": "ok"}


@app.post("/models/rollback", dependencies=[Depends(get_credentials)])
def rollback_model(req: RollbackRequest):
    with Session(engine) as session:
        # delete any promotions for the model
        stmt = select(Promotion).where(Promotion.name == req.name)
        proms = session.exec(stmt).all()
        for p in proms:
            session.delete(p)
        session.commit()
    return {"status": "rolled back", "to_version": req.to_version}



@app.post("/models/metrics", dependencies=[Depends(get_credentials)])
def ingest_metrics(req: MetricsRequest):
    entry = MetricEntry(
        model_name=req.model_name,
        model_version=req.model_version,
        metrics=json.dumps(req.metrics),
    )
    with Session(engine) as session:
        session.add(entry)
        session.commit()
    metric_ingest_counter.inc()
    print(f"stored metrics for {req.model_name}:{req.model_version}")
    return {"status": "accepted"}
