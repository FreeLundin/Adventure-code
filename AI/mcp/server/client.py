from typing import Any, Dict, List, Optional

import httpx

BASE_URL = "http://localhost:8000"


class MPCClient:
    def __init__(self, base_url: str = BASE_URL, api_key: str = "secret"):
        self._client = httpx.Client(base_url=base_url)
        self._api_key = api_key

    def _headers(self):
        return {"X-API-Key": self._api_key}

    def register(self, payload: Dict[str, Any]) -> Dict[str, Any]:
        r = self._client.post("/models/register", json=payload, headers=self._headers())
        r.raise_for_status()
        return r.json()

    def list_models(self, name: Optional[str] = None) -> List[Dict[str, Any]]:
        params = {}
        if name:
            params["name"] = name
        r = self._client.get("/models", params=params, headers=self._headers())
        r.raise_for_status()
        return r.json()

    def get_versions(self, name: str) -> List[Dict[str, Any]]:
        r = self._client.get(f"/models/{name}", headers=self._headers())
        r.raise_for_status()
        return r.json()

    def delete_version(self, name: str, version: str) -> None:
        r = self._client.delete(f"/models/{name}/{version}", headers=self._headers())
        if r.status_code not in (204, 404):
            r.raise_for_status()

    def promote(self, payload: Dict[str, Any]) -> Dict[str, Any]:
        r = self._client.post("/models/promote", json=payload, headers=self._headers())
        r.raise_for_status()
        return r.json()

    def rollback(self, payload: Dict[str, Any]) -> Dict[str, Any]:
        r = self._client.post("/models/rollback", json=payload, headers=self._headers())
        r.raise_for_status()
        return r.json()

    def metrics(self, payload: Dict[str, Any]) -> Dict[str, Any]:
        r = self._client.post("/models/metrics", json=payload, headers=self._headers())
        r.raise_for_status()
        return r.json()

    def search_models(self, query: str) -> List[Dict[str, Any]]:
        r = self._client.get("/models/search", params={"query": query}, headers=self._headers())
        r.raise_for_status()
        return r.json()
