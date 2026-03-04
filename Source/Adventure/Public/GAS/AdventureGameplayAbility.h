// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Gameplay Abilities

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AdventureGameplayAbility.generated.h"

UCLASS(Abstract)
class ADVENTURE_API UAdventureGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UAdventureGameplayAbility();

	/**
	 * Designer‑editable cooldown duration (seconds).
	 * Child ability blueprints can tweak this value per‑ability.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Adventure|Ability")
	float CooldownTime = 0.f;

	/** Called on any ability instance when it becomes active. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Adventure|Ability")
	void BP_OnActivated();

	/** Called when the ability ends. bWasCancelled indicates whether it ended prematurely. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Adventure|Ability")
	void BP_OnEnded(bool bWasCancelled);

protected:
	// override base activation/ending so we can forward to blueprint events
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo *ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData *TriggerEventData) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo *ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility, bool bWasCancelled) override;
};

UCLASS()
class ADVENTURE_API UGA_AdventureSprint : public UAdventureGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_AdventureSprint();
};

UCLASS()
class ADVENTURE_API UGA_AdventureDodge : public UAdventureGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_AdventureDodge();
};

UCLASS()
class ADVENTURE_API UGA_AdventureTraversal : public UAdventureGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_AdventureTraversal();

	// perform traversal logic when the ability activates
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo *ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData *TriggerEventData) override;
};

UCLASS()
class ADVENTURE_API UGA_AdventureClimb : public UAdventureGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_AdventureClimb();

	// invoke climb traversal when activated
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo *ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData *TriggerEventData) override;
};

UCLASS()
class ADVENTURE_API UGA_AdventureInteract : public UAdventureGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_AdventureInteract();
};

UCLASS()
class ADVENTURE_API UGA_AdventureLightAttack : public UAdventureGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_AdventureLightAttack();

	// on activation we grant ritual energy
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo *ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData *TriggerEventData) override;
};

UCLASS()
class ADVENTURE_API UGA_AdventureHeavyAttack : public UAdventureGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_AdventureHeavyAttack();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo *ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData *TriggerEventData) override;
};

// new defensive abilities added in Phase 2
UCLASS()
class ADVENTURE_API UGA_AdventureBlock : public UAdventureGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_AdventureBlock();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo *ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData *TriggerEventData) override;
};

UCLASS()
class ADVENTURE_API UGA_AdventureParry : public UAdventureGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_AdventureParry();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo *ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData *TriggerEventData) override;
};
