// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Gameplay Abilities

#include "GAS/AdventureGameplayAbility.h"
#include "GAS/AdventureGameplayTags.h"

// traversal logic needs access to our character types
#include "Character/CBP_AdventureCharacter.h"
#include "Character/CBP_AdventureCharacter_Mover.h"
#include "Components/CapsuleComponent.h"

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

void UGA_AdventureTraversal::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// when the ability fires we simply forward the request to the character
	if (ActorInfo && ActorInfo->AvatarActor.IsValid())
	{
		// try mover variant first (calls blueprint hooks)
		if (ACBP_AdventureCharacter_Mover* Mover = Cast<ACBP_AdventureCharacter_Mover>(ActorInfo->AvatarActor.Get()))
		{
			// blueprint side will decide vault/mantle/climb based on context
			Mover->RequestTraversalVault();
		}
		else if (ACBP_AdventureCharacter* Char = Cast<ACBP_AdventureCharacter>(ActorInfo->AvatarActor.Get()))
		{
			// fallback for legacy character: perform a simple query that uses forward vector
			FVector Direction = Char->GetLastMovementInputVector();
			if (Direction.IsNearlyZero())
			{
				Direction = Char->GetActorForwardVector();
			}
			FS_TraversalCheckInputs Inputs = Char->GetTraversalCheckInputs(Direction);
			bool bTraversalCheckFailed = true;
			bool bMontageSelectionFailed = true;
			FS_TraversalCheckResult Result;
			Char->TryTraversalAction(Inputs, false, bTraversalCheckFailed, bMontageSelectionFailed, Result,
				Char->GetActorLocation(),
				Char->GetCapsuleComponent()->GetScaledCapsuleRadius(),
				Char->GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
				FVector::ZeroVector, FVector::ZeroVector,
				FHitResult(), 0, 0.0, TArray<UAnimMontage*>());
		}
	}

	// we don't need to stay active, ability is instant
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

UGA_AdventureInteract::UGA_AdventureInteract()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(AdventureGameplayTags::Ability_Interact);
	SetAssetTags(Tags);
}
