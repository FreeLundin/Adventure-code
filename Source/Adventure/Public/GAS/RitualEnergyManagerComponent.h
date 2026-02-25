// Copyright Epic Games, Inc. All Rights Reserved.
// Ritual Energy Manager - Handles decay logic per GDD requirements

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectTypes.h"
#include "ActiveGameplayEffectHandle.h"
#include "RitualEnergyManagerComponent.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

/**
 * URitualEnergyManagerComponent
 * 
 * Manages Ritual Energy decay system for Adventure (Phase 1 deliverable).
 * Per GDD requirements:
 *   - Ritual Energy ranges from 0 to 100
 *   - Decays at -1 per second when out of combat
 *   - Decay begins after 8 seconds of no combat activity
 *   - Stopping decay requires triggering combat state (damage dealt/received, abilities used)
 * 
 * Features:
 *   - Automatic combat state tracking (in-combat vs out-of-combat)
 *   - Configurable decay delay (default: 8 seconds per GDD)
 *   - Configurable decay rate (default: -1/sec per GDD)
 *   - GameplayEffect-based decay for proper GAS integration
 *   - Network replicated for multiplayer consistency
 * 
 * Integration:
 *   - Attaches to ACBP_AdventureCharacter
 *   - Listens to GAS events (OnAbilityActivated, OnDamageReceived, etc.)
 *   - Applies/removes decay GameplayEffect based on combat state
 *   - Exposed to Blueprint for UI feedback (decay warning indicator)
 * 
 * Usage:
 *   1. Add component to ACBP_AdventureCharacter
 *   2. Call Initialize() with character's AbilitySystemComponent
 *   3. Call NotifyCombatAction() whenever player performs combat action
 *   4. Component handles decay automatically based on timer
 * 
 * @see UAdventureAttributeSet for RitualEnergy attribute
 * @see GE_RitualEnergyDecay GameplayEffect asset for decay configuration
 */
UCLASS(ClassGroup=(GAS), meta=(BlueprintSpawnableComponent))
class ADVENTURE_API URitualEnergyManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URitualEnergyManagerComponent();

	// ===== INITIALIZATION =====

	/**
	 * Initialize Ritual Energy manager with character's ASC
	 * Must be called after GAS initialization
	 * 
	 * @param InASC		Character's AbilitySystemComponent
	 */
	UFUNCTION(BlueprintCallable, Category = "Ritual Energy")
	void Initialize(UAbilitySystemComponent* InASC);

	// ===== COMBAT STATE MANAGEMENT =====

	/**
	 * Notify that player performed a combat action (attack, ability, took damage)
	 * Resets the out-of-combat timer and stops decay
	 * 
	 * Called by:
	 *   - Gameplay abilities when activated
	 *   - Damage handlers when taking or dealing damage
	 *   - Parry/dodge/block systems when triggered
	 */
	UFUNCTION(BlueprintCallable, Category = "Ritual Energy")
	void NotifyCombatAction();

	/**
	 * Force start decay immediately (bypasses 8-second timer)
	 * Used for testing or special gameplay scenarios
	 */
	UFUNCTION(BlueprintCallable, Category = "Ritual Energy")
	void ForceStartDecay();

	/**
	 * Force stop decay immediately
	 * Used for cinematics, dialogue, or special gameplay states
	 */
	UFUNCTION(BlueprintCallable, Category = "Ritual Energy")
	void ForceStopDecay();

	// ===== STATE QUERIES =====

	/**
	 * Check if character is currently in combat state
	 * 
	 * @return True if in combat (decay timer not expired), false if out of combat
	 */
	UFUNCTION(BlueprintPure, Category = "Ritual Energy")
	bool IsInCombat() const { return bIsInCombat; }

	/**
	 * Check if Ritual Energy is currently decaying
	 * 
	 * @return True if decay GameplayEffect is active
	 */
	UFUNCTION(BlueprintPure, Category = "Ritual Energy")
	bool IsDecaying() const { return bIsDecaying; }

	/**
	 * Get time remaining until decay starts (seconds)
	 * 
	 * @return Seconds until decay begins (0 if already decaying)
	 */
	UFUNCTION(BlueprintPure, Category = "Ritual Energy")
	float GetTimeUntilDecay() const;

protected:
	// ===== TICK & TIMERS =====

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ===== DECAY LOGIC =====

	/**
	 * Apply RitualEnergy decay GameplayEffect
	 * Called after 8-second out-of-combat timer expires
	 */
	void StartDecay();

	/**
	 * Remove RitualEnergy decay GameplayEffect
	 * Called when combat action occurs
	 */
	void StopDecay();

public:
	// ===== CONFIGURATION =====

	/** Delay before decay starts after last combat action (seconds) - GDD: 8 seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ritual Energy|Config")
	float DecayDelay = 8.0f;

	/** Ritual Energy decay rate per second - GDD: -1/sec */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ritual Energy|Config")
	float DecayRatePerSecond = -1.0f;

	/** GameplayEffect class for Ritual Energy decay (assigned in Blueprint or C++) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ritual Energy|Config")
	TSubclassOf<UGameplayEffect> DecayEffectClass;

private:
	// ===== COMPONENT REFERENCES =====

	/** Reference to character's AbilitySystemComponent */
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent = nullptr;

	// ===== STATE TRACKING =====

	/** Whether character is currently in combat state */
	bool bIsInCombat = false;

	/** Whether decay GameplayEffect is currently active */
	bool bIsDecaying = false;

	/** Time since last combat action (seconds) */
	float TimeSinceLastCombatAction = 0.0f;

	/** Active decay effect handle (for removal) */
	FActiveGameplayEffectHandle DecayEffectHandle;
};
