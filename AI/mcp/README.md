# Model Control Plane (MCP)

This directory contains an OpenAPI specification, a simple Python/FastAPI reference implementation, and utilities for managing model artifacts.

## Structure

- `mcp_openapi.yaml` – OpenAPI 3.0 spec describing the API.
- `server/` – Python FastAPI service implementing the operations (in-memory, replace with real DB).
- `tests/` – pytest-based integration tests against the FastAPI app.
- `cli.py` – lightweight operator CLI built on the HTTP client stub.

## Getting started

The service now uses a SQLite database by default and enforces a simple API key for
access. Override values via environment variables:

* `DATABASE_URL` – SQLAlchemy URL, e.g. `sqlite:///./mcp.db` or `postgresql+psycopg2://user:pass@host/db`
* `API_KEY` – secret used by clients and the CLI (defaults to `secret`).

```powershell
cd AI/mcp/server
python -m venv .venv
.\.venv\Scripts\activate
pip install -r requirements.txt
uvicorn app:app --reload
```

Run the tests from workspace root (they exercise the database and auth header):

```powershell
cd AI/mcp
.\server\.venv\Scripts\activate
pip install pytest
pytest tests
```

## Using OpenAPI Generator

To generate language-specific stubs from the spec:

```bash
openapi-generator-cli generate -i mcp_openapi.yaml -g python-fastapi -o server/generated
openapi-generator-cli generate -i mcp_openapi.yaml -g typescript-axios -o client/ts
```

Adjust commands for your chosen language/framework.

## Next steps

1. **Persistence** – hook `_models` store to a real database (SQLite/Postgres/NoSQL).
2. **Promotions & traffic-split** – endpoints now exist for `POST /models/promote`,
   `POST /models/rollback`, `GET /models/{name}/promotions` and
   `DELETE /models/{name}/promotions`.  Use these to compute routing weights in your
   inference layer or to clear canary deployments.
3. **Metrics storage** – telemetry is saved to a SQLite table; query with
   `GET /models/metrics?model_name=<name>&model_version=<ver>`.
4. **Auth** – simple API‑key middleware (`X-API-Key` header) is enforced; set it via
   `API_KEY` env var and pass `--api-key` to `cli.py` or use generated client.
5. **Model-card validation** – the server attempts to fetch the JSON at
   `model_card_url` and verify that `name`/`version` match; you can extend this to
   validate a schema or integrate with your CI pipeline.
6. **CI pipeline hooks** – add a step in your training job to POST to
   `/models/register` with artifact URLs and computed checksum; see `cli.py` for a
   simple example.
7. **Deployment** – containerize (`Dockerfile`) and deploy to your environment.
8. **UI/CLI** – `cli.py` now accepts `--api-key` and includes a `metrics` command; a
   web dashboard can use the generated client code.

### Azure deployment (Foundry/App Service)

A simple deployment is included under `azure.yaml` and `infra/`. Use
[Azure Developer CLI (azd)](https://learn.microsoft.com/azure/developer/azure-developer-cli/overview) to provision and deploy:

```powershell
cd c:\Unreal_Projects\Adventure
azd auth login
azd env new foundry --subscription <SUB_ID> --location eastus
azd env set AZURE_LOCATION eastus
azd env set DEPLOYER_PRINCIPAL_ID <your-object-id>
azd env set AZURE_ENV_NAME foundry
azd provision --preview

> **Quota warning:** if you see errors about SKU or VM quota (e.g. PremiumV3 or Basic VMs), you may need to choose a lower App Service plan SKU or request a quota increase on your subscription. Modify `infra/resources.bicep` and/or open a support ticket.

azd up
```

The `infra` directory contains Bicep templates for an App Service plan,
Python web app, and RBAC assignments.  The app settings enable a container
build and set Python 3.11.  You can swap to PostgreSQL, Cosmos, etc. by
modifying `infra/resources.bicep`.

> **Note:** "Foundry" here is just the environment name; you can deploy the
> same infrastructure to any resource group or region.  If you need to target
> Azure Foundry (AI/ML platform), replace the `webApp` resource with a
> Container Apps or Kubeflow resource as appropriate.

### Extra steps for an enterprise

* Secure the  `API_KEY` / `JWT_SECRET` in Key Vault and reference via
  `az webapp config appsettings set`.
* Add `COSMOS_CONNECTION` or `POSTGRES_CONNECTION` into `appsettings`.
* Configure CI to run `azd pipeline create` or use GitHub Actions template.

---

> **Advanced ideas**: future enhancements could add full-text search, fine-grained
> access control lists per model or tenant, scheduled (time‑based) rollbacks,
> multi‑tenant isolation, version diffing, or integration with secrets stores for
> API credentials.

See spec for full contract; additional operations (search, tags, etc.) can be added.
