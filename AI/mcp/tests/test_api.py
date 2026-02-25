import pytest
import sys
import os
from fastapi.testclient import TestClient

# ensure server package is importable
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))
from server import app, engine, init_db
from server.app import API_KEY_NAME

client = TestClient(app)
HEADERS = {API_KEY_NAME: "secret"}


def setup_module(module):
    # remove any existing database file and create fresh schema
    try:
        os.remove("mcp.db")
    except Exception:
        pass
    init_db()


def teardown_module(module):
    # remove database file after tests
    try:
        import os
        os.remove("mcp.db")
    except Exception:
        pass


def test_register_and_list():
    payload = {
        "name": "foo",
        "version": "1",
        "model_url": "http://example.com/foo",
        "model_card_url": "http://example.com/foo.json",
    }
    r = client.post("/models/register", headers=HEADERS, json=payload)
    assert r.status_code == 200
    data = r.json()
    assert data["name"] == "foo"

    r = client.get("/models", headers=HEADERS)
    assert r.status_code == 200
    assert any(m["name"] == "foo" for m in r.json())

    # test tag filtering (none present)
    r = client.get("/models?tag=notag", headers=HEADERS)
    assert r.status_code == 200
    assert not r.json()


def test_promote_and_rollback():
    promo = {"name": "foo", "version": "1", "target": "canary", "canary_percent": 5}
    r = client.post("/models/promote", headers=HEADERS, json=promo)
    assert r.status_code == 200

    rb = {"name": "foo", "to_version": "0"}
    r = client.post("/models/rollback", headers=HEADERS, json=rb)
    assert r.status_code == 200

    # query promotions (should be empty after rollback)
    r = client.get(f"/models/foo/promotions", headers=HEADERS)
    assert r.status_code == 200
    assert r.json() == []

    # re-promote and then clear via delete
    r = client.post("/models/promote", headers=HEADERS, json=promo)
    assert r.status_code == 200
    r = client.delete(f"/models/foo/promotions", headers=HEADERS)
    assert r.status_code == 204

    # exercise search endpoint
    r = client.get("/models/search", headers=HEADERS, params={"query": "foo"})
    assert r.status_code == 200
    assert any(m["name"] == "foo" for m in r.json())




def test_metrics_endpoint():
    r = client.post(
        "/models/metrics",
        headers=HEADERS,
        json={
            "model_name": "foo",
            "model_version": "1",
            "metrics": {"latency_ms": 10},
        },
    )
    assert r.status_code == 200

    # query metrics back
    r = client.get("/models/metrics?model_name=foo", headers=HEADERS)
    assert r.status_code == 200
    data = r.json()
    assert data and data[0]["model_name"] == "foo"

    # scraping prometheus endpoint
    r = client.get("/prometheus")
    assert r.status_code == 200
    assert b"mcp_register_total" in r.content
