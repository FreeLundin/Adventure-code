#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Core/AdventureMacros.h"
#include "AdventureBaseAbility.generated.h"

/**
 * Abstract base ability that provides a standard set of parameters and
 * a BlueprintNativeEvent hook for activation.
 * Concrete abilities derive from this class so that designers can
 * author new abilities in Blueprint while still reusing C++ logic.
 */
ADV_CLASS(Abstract)
class ADVENTURE_API UAdventureBaseAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UAdventureBaseAbility();

    /** Amount of Ritual Energy required to activate this ability */
    ADV_PROP(ADV_EXPOSE_ON_SPAWN(), ADV_CLAMP_MIN("0"))
    float RitualCost;

    /**
     * Logic performed when the ability is triggered.  The C++ implementation
     * handles generic bookkeeping; BPs may override to insert custom effects.
     */
    ADV_NATIVE_EVENT()
    void OnActivate();
};
