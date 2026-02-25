from http import HTTPStatus
from typing import Any

import httpx

from ... import errors
from ...client import AuthenticatedClient, Client
from ...models.get_models_metrics_response_200_item import GetModelsMetricsResponse200Item
from ...types import UNSET, Response, Unset


def _get_kwargs(
    *,
    model_name: str | Unset = UNSET,
    model_version: str | Unset = UNSET,
) -> dict[str, Any]:

    params: dict[str, Any] = {}

    params["model_name"] = model_name

    params["model_version"] = model_version

    params = {k: v for k, v in params.items() if v is not UNSET and v is not None}

    _kwargs: dict[str, Any] = {
        "method": "get",
        "url": "/models/metrics",
        "params": params,
    }

    return _kwargs


def _parse_response(
    *, client: AuthenticatedClient | Client, response: httpx.Response
) -> list[GetModelsMetricsResponse200Item] | None:
    if response.status_code == 200:
        response_200 = []
        _response_200 = response.json()
        for response_200_item_data in _response_200:
            response_200_item = GetModelsMetricsResponse200Item.from_dict(response_200_item_data)

            response_200.append(response_200_item)

        return response_200

    if client.raise_on_unexpected_status:
        raise errors.UnexpectedStatus(response.status_code, response.content)
    else:
        return None


def _build_response(
    *, client: AuthenticatedClient | Client, response: httpx.Response
) -> Response[list[GetModelsMetricsResponse200Item]]:
    return Response(
        status_code=HTTPStatus(response.status_code),
        content=response.content,
        headers=response.headers,
        parsed=_parse_response(client=client, response=response),
    )


def sync_detailed(
    *,
    client: AuthenticatedClient | Client,
    model_name: str | Unset = UNSET,
    model_version: str | Unset = UNSET,
) -> Response[list[GetModelsMetricsResponse200Item]]:
    """Query stored metrics

    Args:
        model_name (str | Unset):
        model_version (str | Unset):

    Raises:
        errors.UnexpectedStatus: If the server returns an undocumented status code and Client.raise_on_unexpected_status is True.
        httpx.TimeoutException: If the request takes longer than Client.timeout.

    Returns:
        Response[list[GetModelsMetricsResponse200Item]]
    """

    kwargs = _get_kwargs(
        model_name=model_name,
        model_version=model_version,
    )

    response = client.get_httpx_client().request(
        **kwargs,
    )

    return _build_response(client=client, response=response)


def sync(
    *,
    client: AuthenticatedClient | Client,
    model_name: str | Unset = UNSET,
    model_version: str | Unset = UNSET,
) -> list[GetModelsMetricsResponse200Item] | None:
    """Query stored metrics

    Args:
        model_name (str | Unset):
        model_version (str | Unset):

    Raises:
        errors.UnexpectedStatus: If the server returns an undocumented status code and Client.raise_on_unexpected_status is True.
        httpx.TimeoutException: If the request takes longer than Client.timeout.

    Returns:
        list[GetModelsMetricsResponse200Item]
    """

    return sync_detailed(
        client=client,
        model_name=model_name,
        model_version=model_version,
    ).parsed


async def asyncio_detailed(
    *,
    client: AuthenticatedClient | Client,
    model_name: str | Unset = UNSET,
    model_version: str | Unset = UNSET,
) -> Response[list[GetModelsMetricsResponse200Item]]:
    """Query stored metrics

    Args:
        model_name (str | Unset):
        model_version (str | Unset):

    Raises:
        errors.UnexpectedStatus: If the server returns an undocumented status code and Client.raise_on_unexpected_status is True.
        httpx.TimeoutException: If the request takes longer than Client.timeout.

    Returns:
        Response[list[GetModelsMetricsResponse200Item]]
    """

    kwargs = _get_kwargs(
        model_name=model_name,
        model_version=model_version,
    )

    response = await client.get_async_httpx_client().request(**kwargs)

    return _build_response(client=client, response=response)


async def asyncio(
    *,
    client: AuthenticatedClient | Client,
    model_name: str | Unset = UNSET,
    model_version: str | Unset = UNSET,
) -> list[GetModelsMetricsResponse200Item] | None:
    """Query stored metrics

    Args:
        model_name (str | Unset):
        model_version (str | Unset):

    Raises:
        errors.UnexpectedStatus: If the server returns an undocumented status code and Client.raise_on_unexpected_status is True.
        httpx.TimeoutException: If the request takes longer than Client.timeout.

    Returns:
        list[GetModelsMetricsResponse200Item]
    """

    return (
        await asyncio_detailed(
            client=client,
            model_name=model_name,
            model_version=model_version,
        )
    ).parsed
