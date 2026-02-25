"""A client library for accessing Model Control Plane (MCP) API"""

from .client import AuthenticatedClient, Client

__all__ = (
    "AuthenticatedClient",
    "Client",
)
