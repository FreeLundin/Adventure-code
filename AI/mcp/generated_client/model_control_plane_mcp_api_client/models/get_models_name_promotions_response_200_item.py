from __future__ import annotations

from collections.abc import Mapping
from typing import Any, TypeVar

from attrs import define as _attrs_define
from attrs import field as _attrs_field

from ..types import UNSET, Unset

T = TypeVar("T", bound="GetModelsNamePromotionsResponse200Item")


@_attrs_define
class GetModelsNamePromotionsResponse200Item:
    """
    Attributes:
        version (str | Unset):
        target (str | Unset):
        canary_percent (int | Unset):
    """

    version: str | Unset = UNSET
    target: str | Unset = UNSET
    canary_percent: int | Unset = UNSET
    additional_properties: dict[str, Any] = _attrs_field(init=False, factory=dict)

    def to_dict(self) -> dict[str, Any]:
        version = self.version

        target = self.target

        canary_percent = self.canary_percent

        field_dict: dict[str, Any] = {}
        field_dict.update(self.additional_properties)
        field_dict.update({})
        if version is not UNSET:
            field_dict["version"] = version
        if target is not UNSET:
            field_dict["target"] = target
        if canary_percent is not UNSET:
            field_dict["canary_percent"] = canary_percent

        return field_dict

    @classmethod
    def from_dict(cls: type[T], src_dict: Mapping[str, Any]) -> T:
        d = dict(src_dict)
        version = d.pop("version", UNSET)

        target = d.pop("target", UNSET)

        canary_percent = d.pop("canary_percent", UNSET)

        get_models_name_promotions_response_200_item = cls(
            version=version,
            target=target,
            canary_percent=canary_percent,
        )

        get_models_name_promotions_response_200_item.additional_properties = d
        return get_models_name_promotions_response_200_item

    @property
    def additional_keys(self) -> list[str]:
        return list(self.additional_properties.keys())

    def __getitem__(self, key: str) -> Any:
        return self.additional_properties[key]

    def __setitem__(self, key: str, value: Any) -> None:
        self.additional_properties[key] = value

    def __delitem__(self, key: str) -> None:
        del self.additional_properties[key]

    def __contains__(self, key: str) -> bool:
        return key in self.additional_properties
