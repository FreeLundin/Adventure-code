from __future__ import annotations

from collections.abc import Mapping
from typing import TYPE_CHECKING, Any, TypeVar

from attrs import define as _attrs_define
from attrs import field as _attrs_field

if TYPE_CHECKING:
    from ..models.metrics_request_metrics import MetricsRequestMetrics


T = TypeVar("T", bound="MetricsRequest")


@_attrs_define
class MetricsRequest:
    """
    Attributes:
        model_name (str):
        model_version (str):
        metrics (MetricsRequestMetrics): Example:
            {
              "latency_ms": 12.3,
              "error_rate": 0.01,
              "throughput": 120
            }
    """

    model_name: str
    model_version: str
    metrics: MetricsRequestMetrics
    additional_properties: dict[str, Any] = _attrs_field(init=False, factory=dict)

    def to_dict(self) -> dict[str, Any]:
        model_name = self.model_name

        model_version = self.model_version

        metrics = self.metrics.to_dict()

        field_dict: dict[str, Any] = {}
        field_dict.update(self.additional_properties)
        field_dict.update(
            {
                "model_name": model_name,
                "model_version": model_version,
                "metrics": metrics,
            }
        )

        return field_dict

    @classmethod
    def from_dict(cls: type[T], src_dict: Mapping[str, Any]) -> T:
        from ..models.metrics_request_metrics import MetricsRequestMetrics

        d = dict(src_dict)
        model_name = d.pop("model_name")

        model_version = d.pop("model_version")

        metrics = MetricsRequestMetrics.from_dict(d.pop("metrics"))

        metrics_request = cls(
            model_name=model_name,
            model_version=model_version,
            metrics=metrics,
        )

        metrics_request.additional_properties = d
        return metrics_request

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
