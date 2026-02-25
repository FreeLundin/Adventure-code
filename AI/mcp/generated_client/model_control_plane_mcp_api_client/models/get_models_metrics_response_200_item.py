from __future__ import annotations

import datetime
from collections.abc import Mapping
from typing import TYPE_CHECKING, Any, TypeVar

from attrs import define as _attrs_define
from attrs import field as _attrs_field
from dateutil.parser import isoparse

from ..types import UNSET, Unset

if TYPE_CHECKING:
    from ..models.get_models_metrics_response_200_item_metrics import GetModelsMetricsResponse200ItemMetrics


T = TypeVar("T", bound="GetModelsMetricsResponse200Item")


@_attrs_define
class GetModelsMetricsResponse200Item:
    """
    Attributes:
        model_name (str | Unset):
        model_version (str | Unset):
        metrics (GetModelsMetricsResponse200ItemMetrics | Unset):
        timestamp (datetime.datetime | Unset):
    """

    model_name: str | Unset = UNSET
    model_version: str | Unset = UNSET
    metrics: GetModelsMetricsResponse200ItemMetrics | Unset = UNSET
    timestamp: datetime.datetime | Unset = UNSET
    additional_properties: dict[str, Any] = _attrs_field(init=False, factory=dict)

    def to_dict(self) -> dict[str, Any]:
        model_name = self.model_name

        model_version = self.model_version

        metrics: dict[str, Any] | Unset = UNSET
        if not isinstance(self.metrics, Unset):
            metrics = self.metrics.to_dict()

        timestamp: str | Unset = UNSET
        if not isinstance(self.timestamp, Unset):
            timestamp = self.timestamp.isoformat()

        field_dict: dict[str, Any] = {}
        field_dict.update(self.additional_properties)
        field_dict.update({})
        if model_name is not UNSET:
            field_dict["model_name"] = model_name
        if model_version is not UNSET:
            field_dict["model_version"] = model_version
        if metrics is not UNSET:
            field_dict["metrics"] = metrics
        if timestamp is not UNSET:
            field_dict["timestamp"] = timestamp

        return field_dict

    @classmethod
    def from_dict(cls: type[T], src_dict: Mapping[str, Any]) -> T:
        from ..models.get_models_metrics_response_200_item_metrics import GetModelsMetricsResponse200ItemMetrics

        d = dict(src_dict)
        model_name = d.pop("model_name", UNSET)

        model_version = d.pop("model_version", UNSET)

        _metrics = d.pop("metrics", UNSET)
        metrics: GetModelsMetricsResponse200ItemMetrics | Unset
        if isinstance(_metrics, Unset):
            metrics = UNSET
        else:
            metrics = GetModelsMetricsResponse200ItemMetrics.from_dict(_metrics)

        _timestamp = d.pop("timestamp", UNSET)
        timestamp: datetime.datetime | Unset
        if isinstance(_timestamp, Unset):
            timestamp = UNSET
        else:
            timestamp = isoparse(_timestamp)

        get_models_metrics_response_200_item = cls(
            model_name=model_name,
            model_version=model_version,
            metrics=metrics,
            timestamp=timestamp,
        )

        get_models_metrics_response_200_item.additional_properties = d
        return get_models_metrics_response_200_item

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
