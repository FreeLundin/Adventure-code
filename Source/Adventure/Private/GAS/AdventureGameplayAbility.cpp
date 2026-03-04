// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Gameplay Abilities

#include "GAS/AdventureGameplayAbility.h"
#include "GAS/AdventureGameplayTags.h"
#include "GAS/AdventureAttributeSet.h"
#include "Core/AdventurePlayerState.h"

// traversal logic needs access to our character types
#include "Character/CBP_AdventureCharacter.h"
#include "Character/CBP_AdventureCharacter_Mover.h"
#include "Components/CapsuleComponent.h"

UAdventureGameplayAbility::UAdventureGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UAdventureGameplayAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo *ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData *TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// notify player state about combat action (reset decay timer)
	if (ActorInfo && ActorInfo->OwnerActor.IsValid())
	{
		if (AAdventurePlayerState *PS = Cast<AAdventurePlayerState>(ActorInfo->OwnerActor.Get()))
		{
			PS->OnAbilityUsed();
		}
	}

	// forward to blueprint if implemented
	BP_OnActivated();
}

void UAdventureGameplayAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo *ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// notify blueprint
	BP_OnEnded(bWasCancelled);
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

UGA_AdventureClimb::UGA_AdventureClimb()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(AdventureGameplayTags::Ability_Climb);
	SetAssetTags(Tags);
}

UGA_AdventureLightAttack::UGA_AdventureLightAttack()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(AdventureGameplayTags::Ability_Attack_Light);
	SetAssetTags(Tags);
}

UGA_AdventureHeavyAttack::UGA_AdventureHeavyAttack()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(AdventureGameplayTags::Ability_Attack_Heavy);
	SetAssetTags(Tags);
}

UGA_AdventureBlock::UGA_AdventureBlock()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(AdventureGameplayTags::Ability_Attack_Block);
	SetAssetTags(Tags);
}

UGA_AdventureParry::UGA_AdventureParry()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(AdventureGameplayTags::Ability_Attack_Parry);
	SetAssetTags(Tags);
}



void UGA_AdventureTraversal::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo *ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData *TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// when the ability fires we simply forward the request to the character
	if (ActorInfo && ActorInfo->AvatarActor.IsValid())
	{
		// try mover variant first (calls blueprint hooks)
		if (ACBP_AdventureCharacter_Mover *Mover = Cast<ACBP_AdventureCharacter_Mover>(ActorInfo->AvatarActor.Get()))
		{
			// blueprint side will decide vault/mantle/climb based on context
			Mover->RequestTraversalVault();
		}
		else if (ACBP_AdventureCharacter *Char = Cast<ACBP_AdventureCharacter>(ActorInfo->AvatarActor.Get()))
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
									 FHitResult(), 0, 0.0, TArray<UAnimMontage *>());
		}
	}

	// we don't need to stay active, ability is instant
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_AdventureClimb::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo *ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData *TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ActorInfo && ActorInfo->AvatarActor.IsValid())
	{
		if (ACBP_AdventureCharacter_Mover *Mover = Cast<ACBP_AdventureCharacter_Mover>(ActorInfo->AvatarActor.Get()))
		{
			Mover->RequestTraversalClimb();
		}
		else if (ACBP_AdventureCharacter *Char = Cast<ACBP_AdventureCharacter>(ActorInfo->AvatarActor.Get()))
		{
			// legacy fallback uses same generic Traversal query but forces climb type
			Char->OnTraversalRequestClimb_Implementation();
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_AdventureLightAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo *ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData *TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		FGameplayAttribute Attr = UAdventureAttributeSet::GetRitualEnergyAttribute();
		ActorInfo->AbilitySystemComponent->ApplyModToAttribute(Attr, EGameplayModOp::Additive, 3.0f);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_AdventureHeavyAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo *ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData *TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		FGameplayAttribute Attr = UAdventureAttributeSet::GetRitualEnergyAttribute();
		ActorInfo->AbilitySystemComponent->ApplyModToAttribute(Attr, EGameplayModOp::Additive, 8.0f);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_AdventureBlock::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo *ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData *TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// block grants a small amount of ritual energy for successful defense
	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		FGameplayAttribute Attr = UAdventureAttributeSet::GetRitualEnergyAttribute();
		ActorInfo->AbilitySystemComponent->ApplyModToAttribute(Attr, EGameplayModOp::Additive, 1.0f);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_AdventureParry::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo *ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData *TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// parry gives a moderate energy bonus on perfect timing
	if (ActorInfo && ActorInfo->AbilitySystemComponent.IsValid())
	{
		FGameplayAttribute Attr = UAdventureAttributeSet::GetRitualEnergyAttribute();
		ActorInfo->AbilitySystemComponent->ApplyModToAttribute(Attr, EGameplayModOp::Additive, 5.0f);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}