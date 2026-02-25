from enum import Enum


class PromoteModelRequestTarget(str, Enum):
    CANARY = "canary"
    STABLE = "stable"

    def __str__(self) -> str:
        return str(self.value)
