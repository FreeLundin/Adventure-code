#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "AdventureGASStateTreeBridgeComponent.generated.h"

class UAbilitySystemComponent;

UCLASS(ClassGroup=(Adventure), meta=(BlueprintSpawnableComponent))
class ADVENTURE_API UAdventureGASStateTreeBridgeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAdventureGASStateTreeBridgeComponent();

    virtual void BeginPlay() override;

    // --- StateTree-facing read API ---
    UFUNCTION(BlueprintCallable, Category="Adventure|StateTree")
    bool IsTraversalActive() const { return bTraversalActive; }

    UFUNCTION(BlueprintCallable, Category="Adventure|StateTree")
    FGameplayTag GetActiveTraversalType() const { return ActiveTraversalType; }

    UFUNCTION(BlueprintCallable, Category="Adventure|StateTree")
    bool IsInWater() const { return bInWater; }

    // --- Inputs from other systems (e.g., water volumes / mover mode callbacks) ---
    UFUNCTION(BlueprintCallable, Category="Adventure|StateTree")
    void SetInWater(bool bNewInWater) { bInWater = bNewInWater; }

protected:
    // Finds ASC on PlayerState (works with PlayerState-owned ASC)
    UAbilitySystemComponent* ResolveASC() const;

    // Tag callbacks
    void BindASCDelegates(UAbilitySystemComponent* ASC);
    void OnTraversalActiveTagChanged(const FGameplayTag Tag, int32 NewCount);
    void OnClimbTypeTagChanged(const FGameplayTag Tag, int32 NewCount);
    void OnMantleTypeTagChanged(const FGameplayTag Tag, int32 NewCount);
    void OnSwimExitTypeTagChanged(const FGameplayTag Tag, int32 NewCount);

private:
    UPROPERTY(VisibleAnywhere, Category="Adventure|StateTree")
    bool bTraversalActive = false;

    UPROPERTY(VisibleAnywhere, Category="Adventure|StateTree")
    bool bInWater = false;

    UPROPERTY(VisibleAnywhere, Category="Adventure|StateTree")
    FGameplayTag ActiveTraversalType;

    // Cached tags
    FGameplayTag Tag_TraversalActive;
    FGameplayTag Tag_TypeClimb;
    FGameplayTag Tag_TypeMantle;
    FGameplayTag Tag_TypeSwimExit;
};
