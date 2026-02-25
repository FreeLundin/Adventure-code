from __future__ import annotations

import datetime
from collections.abc import Mapping
from typing import TYPE_CHECKING, Any, TypeVar, cast

from attrs import define as _attrs_define
from attrs import field as _attrs_field
from dateutil.parser import isoparse

from ..types import UNSET, Unset

if TYPE_CHECKING:
    from ..models.model_card_metadata import ModelCardMetadata
    from ..models.model_card_provenance import ModelCardProvenance


T = TypeVar("T", bound="ModelCard")


@_attrs_define
class ModelCard:
    """
    Attributes:
        name (str):
        version (str):
        description (str | Unset): A brief description of the model's purpose and capabilities
        owner (str | Unset): The team or individual responsible for the model
        tenant (str | Unset): Tenant/namespace owning the model (for ACLs)
        license_ (str | Unset): License under which the model is released
        tags (list[str] | Unset): Keywords or categories associated with the model; used by the API for filtering/search
        created_at (datetime.datetime | Unset):
        updated_at (datetime.datetime | Unset):
        provenance (ModelCardProvenance | Unset): Additional metadata about model lineage (training data, source repo,
            etc.)
        metadata (ModelCardMetadata | Unset): Arbitrary key/value data provided at registration
    """

    name: str
    version: str
    description: str | Unset = UNSET
    owner: str | Unset = UNSET
    tenant: str | Unset = UNSET
    license_: str | Unset = UNSET
    tags: list[str] | Unset = UNSET
    created_at: datetime.datetime | Unset = UNSET
    updated_at: datetime.datetime | Unset = UNSET
    provenance: ModelCardProvenance | Unset = UNSET
    metadata: ModelCardMetadata | Unset = UNSET
    additional_properties: dict[str, Any] = _attrs_field(init=False, factory=dict)

    def to_dict(self) -> dict[str, Any]:
        name = self.name

        version = self.version

        description = self.description

        owner = self.owner

        tenant = self.tenant

        license_ = self.license_

        tags: list[str] | Unset = UNSET
        if not isinstance(self.tags, Unset):
            tags = self.tags

        created_at: str | Unset = UNSET
        if not isinstance(self.created_at, Unset):
            created_at = self.created_at.isoformat()

        updated_at: str | Unset = UNSET
        if not isinstance(self.updated_at, Unset):
            updated_at = self.updated_at.isoformat()

        provenance: dict[str, Any] | Unset = UNSET
        if not isinstance(self.provenance, Unset):
            provenance = self.provenance.to_dict()

        metadata: dict[str, Any] | Unset = UNSET
        if not isinstance(self.metadata, Unset):
            metadata = self.metadata.to_dict()

        field_dict: dict[str, Any] = {}
        field_dict.update(self.additional_properties)
        field_dict.update(
            {
                "name": name,
                "version": version,
            }
        )
        if description is not UNSET:
            field_dict["description"] = description
        if owner is not UNSET:
            field_dict["owner"] = owner
        if tenant is not UNSET:
            field_dict["tenant"] = tenant
        if license_ is not UNSET:
            field_dict["license"] = license_
        if tags is not UNSET:
            field_dict["tags"] = tags
        if created_at is not UNSET:
            field_dict["created_at"] = created_at
        if updated_at is not UNSET:
            field_dict["updated_at"] = updated_at
        if provenance is not UNSET:
            field_dict["provenance"] = provenance
        if metadata is not UNSET:
            field_dict["metadata"] = metadata

        return field_dict

    @classmethod
    def from_dict(cls: type[T], src_dict: Mapping[str, Any]) -> T:
        from ..models.model_card_metadata import ModelCardMetadata
        from ..models.model_card_provenance import ModelCardProvenance

        d = dict(src_dict)
        name = d.pop("name")

        version = d.pop("version")

        description = d.pop("description", UNSET)

        owner = d.pop("owner", UNSET)

        tenant = d.pop("tenant", UNSET)

        license_ = d.pop("license", UNSET)

        tags = cast(list[str], d.pop("tags", UNSET))

        _created_at = d.pop("created_at", UNSET)
        created_at: datetime.datetime | Unset
        if isinstance(_created_at, Unset):
            created_at = UNSET
        else:
            created_at = isoparse(_created_at)

        _updated_at = d.pop("updated_at", UNSET)
        updated_at: datetime.datetime | Unset
        if isinstance(_updated_at, Unset):
            updated_at = UNSET
        else:
            updated_at = isoparse(_updated_at)

        _provenance = d.pop("provenance", UNSET)
        provenance: ModelCardProvenance | Unset
        if isinstance(_provenance, Unset):
            provenance = UNSET
        else:
            provenance = ModelCardProvenance.from_dict(_provenance)

        _metadata = d.pop("metadata", UNSET)
        metadata: ModelCardMetadata | Unset
        if isinstance(_metadata, Unset):
            metadata = UNSET
        else:
            metadata = ModelCardMetadata.from_dict(_metadata)

        model_card = cls(
            name=name,
            version=version,
            description=description,
            owner=owner,
            tenant=tenant,
            license_=license_,
            tags=tags,
            created_at=created_at,
            updated_at=updated_at,
            provenance=provenance,
            metadata=metadata,
        )

        model_card.additional_properties = d
        return model_card

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
