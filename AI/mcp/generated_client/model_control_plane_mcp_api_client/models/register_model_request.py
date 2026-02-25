from __future__ import annotations

from collections.abc import Mapping
from typing import TYPE_CHECKING, Any, TypeVar

from attrs import define as _attrs_define
from attrs import field as _attrs_field

from ..types import UNSET, Unset

if TYPE_CHECKING:
    from ..models.register_model_request_metadata import RegisterModelRequestMetadata


T = TypeVar("T", bound="RegisterModelRequest")


@_attrs_define
class RegisterModelRequest:
    """
    Attributes:
        name (str):
        version (str):
        model_url (str):
        model_card_url (str):
        checksum (str | Unset):
        metadata (RegisterModelRequestMetadata | Unset):
    """

    name: str
    version: str
    model_url: str
    model_card_url: str
    checksum: str | Unset = UNSET
    metadata: RegisterModelRequestMetadata | Unset = UNSET
    additional_properties: dict[str, Any] = _attrs_field(init=False, factory=dict)

    def to_dict(self) -> dict[str, Any]:
        name = self.name

        version = self.version

        model_url = self.model_url

        model_card_url = self.model_card_url

        checksum = self.checksum

        metadata: dict[str, Any] | Unset = UNSET
        if not isinstance(self.metadata, Unset):
            metadata = self.metadata.to_dict()

        field_dict: dict[str, Any] = {}
        field_dict.update(self.additional_properties)
        field_dict.update(
            {
                "name": name,
                "version": version,
                "model_url": model_url,
                "model_card_url": model_card_url,
            }
        )
        if checksum is not UNSET:
            field_dict["checksum"] = checksum
        if metadata is not UNSET:
            field_dict["metadata"] = metadata

        return field_dict

    @classmethod
    def from_dict(cls: type[T], src_dict: Mapping[str, Any]) -> T:
        from ..models.register_model_request_metadata import RegisterModelRequestMetadata

        d = dict(src_dict)
        name = d.pop("name")

        version = d.pop("version")

        model_url = d.pop("model_url")

        model_card_url = d.pop("model_card_url")

        checksum = d.pop("checksum", UNSET)

        _metadata = d.pop("metadata", UNSET)
        metadata: RegisterModelRequestMetadata | Unset
        if isinstance(_metadata, Unset):
            metadata = UNSET
        else:
            metadata = RegisterModelRequestMetadata.from_dict(_metadata)

        register_model_request = cls(
            name=name,
            version=version,
            model_url=model_url,
            model_card_url=model_card_url,
            checksum=checksum,
            metadata=metadata,
        )

        register_model_request.additional_properties = d
        return register_model_request

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
