// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Gameplay Abilities

#include "GAS/AdventureGameplayAbility.h"
#include "GAS/AdventureGameplayTags.h"

UAdventureGameplayAbility::UAdventureGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

UGA_AdventureSprint::UGA_AdventureSprint()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(AdventureGameplayTags::Ability_Sprint);
	SetAssetTags(Tags);
}

UGA_AdventureDodge::UGA_AdventureDodge()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(AdventureGameplayTags::Ability_Dodge);
	SetAssetTags(Tags);
}

UGA_AdventureTraversal::UGA_AdventureTraversal()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(AdventureGameplayTags::Ability_Traversal);
	SetAssetTags(Tags);
}

UGA_AdventureInteract::UGA_AdventureInteract()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(AdventureGameplayTags::Ability_Interact);
	SetAssetTags(Tags);
}
