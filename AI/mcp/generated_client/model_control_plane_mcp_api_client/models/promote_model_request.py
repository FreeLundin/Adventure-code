from __future__ import annotations

from collections.abc import Mapping
from typing import Any, TypeVar

from attrs import define as _attrs_define
from attrs import field as _attrs_field

from ..models.promote_model_request_target import PromoteModelRequestTarget
from ..types import UNSET, Unset

T = TypeVar("T", bound="PromoteModelRequest")


@_attrs_define
class PromoteModelRequest:
    """
    Attributes:
        name (str):
        version (str):
        target (PromoteModelRequestTarget):
        canary_percent (int | Unset):
    """

    name: str
    version: str
    target: PromoteModelRequestTarget
    canary_percent: int | Unset = UNSET
    additional_properties: dict[str, Any] = _attrs_field(init=False, factory=dict)

    def to_dict(self) -> dict[str, Any]:
        name = self.name

        version = self.version

        target = self.target.value

        canary_percent = self.canary_percent

        field_dict: dict[str, Any] = {}
        field_dict.update(self.additional_properties)
        field_dict.update(
            {
                "name": name,
                "version": version,
                "target": target,
            }
        )
        if canary_percent is not UNSET:
            field_dict["canary_percent"] = canary_percent

        return field_dict

    @classmethod
    def from_dict(cls: type[T], src_dict: Mapping[str, Any]) -> T:
        d = dict(src_dict)
        name = d.pop("name")

        version = d.pop("version")

        target = PromoteModelRequestTarget(d.pop("target"))

        canary_percent = d.pop("canary_percent", UNSET)

        promote_model_request = cls(
            name=name,
            version=version,
            target=target,
            canary_percent=canary_percent,
        )

        promote_model_request.additional_properties = d
        return promote_model_request

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
