#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AdventureHUDWidget.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "AdventureHUDComponent.generated.h"

class UAbilitySystemComponent;
class UAdventureAttributeSet;
class IAbilitySystemInterface;

/**
 * Component that automatically creates a HUD widget and binds it to
 * attributes on the owner's AbilitySystemComponent.  Drop this on a
 * PlayerController (or Pawn) and set HUDClass; the component handles
 * wiring health/stamina/magic updates for you.
 */
UCLASS(ClassGroup = (UI), meta = (BlueprintSpawnableComponent))
class ADVENTURE_API UAdventureHUDComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAdventureHUDComponent();

    /** Widget class to instantiate and add to viewport. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    TSubclassOf<UAdventureHUDWidget> HUDClass;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UAdventureHUDWidget *HUDWidget;

    // callbacks for attribute changes
    void OnHealthChanged(const FOnAttributeChangeData &Data);

    void OnStaminaChanged(const FOnAttributeChangeData &Data);

    void OnMagicChanged(const FOnAttributeChangeData &Data);

    void OnRageChanged(const FOnAttributeChangeData &Data);

    void OnShieldChanged(const FOnAttributeChangeData &Data);

    const UAdventureAttributeSet *GetAttributes() const;
    UAbilitySystemComponent *GetASC() const;
};