from http import HTTPStatus
from typing import Any

import httpx

from ... import errors
from ...client import AuthenticatedClient, Client
from ...models.model_card import ModelCard
from ...types import UNSET, Response, Unset


def _get_kwargs(
    *,
    name: str | Unset = UNSET,
    tag: str | Unset = UNSET,
) -> dict[str, Any]:

    params: dict[str, Any] = {}

    params["name"] = name

    params["tag"] = tag

    params = {k: v for k, v in params.items() if v is not UNSET and v is not None}

    _kwargs: dict[str, Any] = {
        "method": "get",
        "url": "/models",
        "params": params,
    }

    return _kwargs


def _parse_response(*, client: AuthenticatedClient | Client, response: httpx.Response) -> list[ModelCard] | None:
    if response.status_code == 200:
        response_200 = []
        _response_200 = response.json()
        for response_200_item_data in _response_200:
            response_200_item = ModelCard.from_dict(response_200_item_data)

            response_200.append(response_200_item)

        return response_200

    if client.raise_on_unexpected_status:
        raise errors.UnexpectedStatus(response.status_code, response.content)
    else:
        return None


def _build_response(*, client: AuthenticatedClient | Client, response: httpx.Response) -> Response[list[ModelCard]]:
    return Response(
        status_code=HTTPStatus(response.status_code),
        content=response.content,
        headers=response.headers,
        parsed=_parse_response(client=client, response=response),
    )


def sync_detailed(
    *,
    client: AuthenticatedClient | Client,
    name: str | Unset = UNSET,
    tag: str | Unset = UNSET,
) -> Response[list[ModelCard]]:
    """List registered models

    Args:
        name (str | Unset):
        tag (str | Unset):

    Raises:
        errors.UnexpectedStatus: If the server returns an undocumented status code and Client.raise_on_unexpected_status is True.
        httpx.TimeoutException: If the request takes longer than Client.timeout.

    Returns:
        Response[list[ModelCard]]
    """

    kwargs = _get_kwargs(
        name=name,
        tag=tag,
    )

    response = client.get_httpx_client().request(
        **kwargs,
    )

    return _build_response(client=client, response=response)


def sync(
    *,
    client: AuthenticatedClient | Client,
    name: str | Unset = UNSET,
    tag: str | Unset = UNSET,
) -> list[ModelCard] | None:
    """List registered models

    Args:
        name (str | Unset):
        tag (str | Unset):

    Raises:
        errors.UnexpectedStatus: If the server returns an undocumented status code and Client.raise_on_unexpected_status is True.
        httpx.TimeoutException: If the request takes longer than Client.timeout.

    Returns:
        list[ModelCard]
    """

    return sync_detailed(
        client=client,
        name=name,
        tag=tag,
    ).parsed


async def asyncio_detailed(
    *,
    client: AuthenticatedClient | Client,
    name: str | Unset = UNSET,
    tag: str | Unset = UNSET,
) -> Response[list[ModelCard]]:
    """List registered models

    Args:
        name (str | Unset):
        tag (str | Unset):

    Raises:
        errors.UnexpectedStatus: If the server returns an undocumented status code and Client.raise_on_unexpected_status is True.
        httpx.TimeoutException: If the request takes longer than Client.timeout.

    Returns:
        Response[list[ModelCard]]
    """

    kwargs = _get_kwargs(
        name=name,
        tag=tag,
    )

    response = await client.get_async_httpx_client().request(**kwargs)

    return _build_response(client=client, response=response)


async def asyncio(
    *,
    client: AuthenticatedClient | Client,
    name: str | Unset = UNSET,
    tag: str | Unset = UNSET,
) -> list[ModelCard] | None:
    """List registered models

    Args:
        name (str | Unset):
        tag (str | Unset):

    Raises:
        errors.UnexpectedStatus: If the server returns an undocumented status code and Client.raise_on_unexpected_status is True.
        httpx.TimeoutException: If the request takes longer than Client.timeout.

    Returns:
        list[ModelCard]
    """

    return (
        await asyncio_detailed(
            client=client,
            name=name,
            tag=tag,
        )
    ).parsed
