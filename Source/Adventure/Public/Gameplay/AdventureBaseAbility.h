#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AdventureBaseAbility.generated.h"

UCLASS()
class ADVENTURE_API UAdventureBaseAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    // Constructor
    UAdventureBaseAbility();

    // Ritual cost for this ability
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    float RitualCost = 0.0f;

    // Optional blueprint-callable activation hook
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability")
    void OnActivate();
    virtual void OnActivate_Implementation();
};
