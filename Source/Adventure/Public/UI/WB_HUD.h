// Copyright Epic Games, Inc. All Rights Reserved.
// HUD Widget for Adventure — Main UI overlay displaying player attributes

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "AttributeSet.h"
#include "GAS/AdventureAttributeSet.h"
#include "WB_HUD.generated.h"

class APC_AdventureController;
class ACBP_AdventureCharacter;
class UAdventureAttributeSet;

/**
 * UWB_HUD
 * 
 * Main HUD widget for Adventure displaying real-time player attributes.
 * 
 * Features:
 *   - Health bar (red) with current/max health text
 *   - Stamina bar (green) with current/max stamina
 *   - Ritual Energy bar (purple) with current/max ritual energy
 *   - Ammo counter (if weapon equipped)
 *   - Ability cooldown indicators (future)
 *   - Artifact status icons (future)
 * 
 * Binding Strategy:
 *   1. Widget obtains reference to player's AttributeSet (via PlayerState ASC)
 *   2. Widget subscribes to OnRep callbacks for each attribute
 *   3. OnRep triggers UpdateUI() to refresh bars, text, and effects
 *   4. Supports multiple attribute updates per frame (batch UI updates via Timer)
 * 
 * Integration:
 *   - Instantiated in APC_AdventureController::BeginPlay()
 *   - Added to viewport once player possesses character
 *   - Survives level transitions (optionally cached in PlayerState)
 * 
 * @see APC_AdventureController
 * @see UAdventureAttributeSet
 * @see AAdventurePlayerState
 */
UCLASS()
class ADVENTURE_API UWB_HUD : public UUserWidget
{
	GENERATED_BODY()

public:
	// ===== INITIALIZATION =====

	/**
	 * Initialize widget with player's attribute set
	 * Called after widget is created and added to viewport
	 */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void InitializeHUD(APC_AdventureController* OwnerController);

	/**
	 * Unbind all attribute callbacks and cleanup
	 * Called when widget is removed from viewport or player dies
	 */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShutdownHUD();

	// ===== ATTRIBUTE UPDATE CALLBACKS =====

	/**
	 * Called when Health attribute changes (via GAS delegate)
	 * Updates health bar and text display
	 */
	void OnHealthChanged(const FOnAttributeChangeData& Data);

	/**
	 * Called when Stamina attribute changes (via GAS delegate)
	 * Updates stamina bar and text display
	 */
	void OnStaminaChanged(const FOnAttributeChangeData& Data);

	/**
	 * Called when RitualEnergy attribute changes (via GAS delegate)
	 * Updates ritual energy bar and glow effect
	 */
	void OnRitualEnergyChanged(const FOnAttributeChangeData& Data);

	// ===== UI UPDATE METHODS =====

	/**
	 * Update all attribute bars with current values
	 * Called during initialization and after attribute changes
	 */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void RefreshAllBars();

	/**
	 * Set health bar fill percentage and text
	 * @param CurrentHealth		Current health value
	 * @param MaxHealth			Max health for the character
	 */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateHealthBar(float CurrentHealth, float MaxHealth);

	/**
	 * Set stamina bar fill percentage and text
	 * @param CurrentStamina	Current stamina value
	 * @param MaxStamina		Max stamina for the character
	 */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateStaminaBar(float CurrentStamina, float MaxStamina);

	/**
	 * Set ritual energy bar fill percentage, text, and glow effect
	 * @param CurrentRitual		Current ritual energy value
	 * @param MaxRitual			Max ritual energy (typically 100)
	 */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateRitualEnergyBar(float CurrentRitual, float MaxRitual);

	// ===== WIDGET COMPONENTS (Editable in Blueprint) =====

	/** Health bar progress widget */
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar = nullptr;

	/** Health text display (e.g., "100 / 100") */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText = nullptr;

	/** Stamina bar progress widget */
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar = nullptr;

	/** Stamina text display (e.g., "100 / 100") */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StaminaText = nullptr;

	/** Ritual Energy bar progress widget */
	UPROPERTY(meta = (BindWidget))
	UProgressBar* RitualEnergyBar = nullptr;

	/** Ritual Energy text display (e.g., "100 / 100") */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RitualEnergyText = nullptr;

	// ===== CONFIGURATION (Editable in Blueprint) =====

	/** Color of health bar when full (default: bright red) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Colors")
	FLinearColor HealthBarColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f); // Red

	/** Color of stamina bar when full (default: bright green) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Colors")
	FLinearColor StaminaBarColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f); // Green

	/** Color of ritual energy bar when full (default: purple) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Colors")
	FLinearColor RitualEnergyBarColor = FLinearColor(0.8f, 0.0f, 1.0f, 1.0f); // Purple

	/** Low health threshold (bar turns orange/red) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Thresholds")
	float LowHealthThreshold = 30.0f; // Warn when health < 30%

	/** Color warning tint for low health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Colors")
	FLinearColor LowHealthWarningColor = FLinearColor(1.0f, 0.5f, 0.0f, 1.0f); // Orange

private:
	// ===== INTERNAL STATE =====

	/** Cached reference to player's AttributeSet */
	UPROPERTY()
	UAdventureAttributeSet* CachedAttributeSet = nullptr;

	/** Cached reference to player character */
	UPROPERTY()
	ACBP_AdventureCharacter* CachedCharacter = nullptr;

	/** Flag to prevent recursive updates */
	bool bUpdatingUI = false;

	// ===== HELPER METHODS =====

	/**
	 * Get the player's AttributeSet from the current PlayerState ASC
	 * @return Pointer to AttributeSet if available, nullptr otherwise
	 */
	UAdventureAttributeSet* GetCharacterAttributeSet() const;

	/**
	 * Bind this widget to AttributeSet callbacks
	 * Called during InitializeHUD
	 */
	void BindToAttributeSet();

	/**
	 * Unbind from AttributeSet callbacks
	 * Called during ShutdownHUD
	 */
	void UnbindFromAttributeSet();

	/**
	 * Calculate bar fill percentage (0.0 to 1.0)
	 * @param Current	Current attribute value
	 * @param Max		Max attribute value
	 * @return Normalized percentage (0-1)
	 */
	static float GetBarFillPercentage(float Current, float Max);

	/**
	 * Format attribute text (e.g., "100.0 / 100.0")
	 * @param Current	Current value
	 * @param Max		Max value
	 * @return Formatted string
	 */
	static FString FormatAttributeText(float Current, float Max);

public:
	// ===== NATIVE EVENT OVERRIDES =====

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
