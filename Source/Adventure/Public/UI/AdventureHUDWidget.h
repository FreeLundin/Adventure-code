#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AdventureHUDWidget.generated.h"

/**
 * Base HUD widget for Adventure.  Blueprint subclasses should bind visuals to the
 * exposed events/slots.
 */
UCLASS()
class ADVENTURE_API UAdventureHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /** Update current Ritual Energy (0-100). */
    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void SetRitualEnergy(float NewValue);

    /** Display current camera mode name (TopDown/ThirdPerson/FirstPerson) */
    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void SetCameraMode(const FString &ModeName);

    /** Update Rage bar. */
    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void SetRage(float NewValue, float MaxValue);

    /** Update Shield bar. */
    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void SetShield(float NewValue, float MaxValue);

    /** Update Health bar. */
    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void SetHealth(float NewHealth, float MaxHealth);

    /** Update Stamina bar. */
    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void SetStamina(float NewStamina, float MaxStamina);

    /** Update ammo count or similar. */
    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void SetAmmo(int32 CurrentAmmo, int32 MaxAmmo);

    /** Show/hide pause menu overlay */
    UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
    void SetPauseMenuVisible(bool bVisible);
};