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
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
};

UCLASS()
class ADVENTURE_API UGA_AdventureInteract : public UAdventureGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_AdventureInteract();
};
