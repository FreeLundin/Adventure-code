// Copyright Epic Games, Inc. All Rights Reserved.
// Ritual Energy Manager - Implementation

#include "GAS/RitualEnergyManagerComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GAS/AdventureAttributeSet.h"

URitualEnergyManagerComponent::URitualEnergyManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsInCombat = false;
	bIsDecaying = false;
	TimeSinceLastCombatAction = 0.0f;
}

void URitualEnergyManagerComponent::Initialize(UAbilitySystemComponent* InASC)
{
	AbilitySystemComponent = InASC;

	if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("URitualEnergyManagerComponent::Initialize - Invalid AbilitySystemComponent"));
		return;
	}

	// Start in combat state (no immediate decay)
	bIsInCombat = true;
	TimeSinceLastCombatAction = 0.0f;

	UE_LOG(LogTemp, Log, TEXT("URitualEnergyManagerComponent::Initialize - Ritual Energy manager initialized (Decay delay: %.1fs, Decay rate: %.1f/sec)"),
		DecayDelay, DecayRatePerSecond);
}

void URitualEnergyManagerComponent::NotifyCombatAction()
{
	// Reset combat timer
	bIsInCombat = true;
	TimeSinceLastCombatAction = 0.0f;

	// Stop decay if active
	if (bIsDecaying)
	{
		StopDecay();
	}

	UE_LOG(LogTemp, Verbose, TEXT("URitualEnergyManagerComponent::NotifyCombatAction - Combat action detected, decay timer reset"));
}

void URitualEnergyManagerComponent::ForceStartDecay()
{
	if (!bIsDecaying)
	{
		StartDecay();
		UE_LOG(LogTemp, Log, TEXT("URitualEnergyManagerComponent::ForceStartDecay - Decay forced to start"));
	}
}

void URitualEnergyManagerComponent::ForceStopDecay()
{
	if (bIsDecaying)
	{
		StopDecay();
		UE_LOG(LogTemp, Log, TEXT("URitualEnergyManagerComponent::ForceStopDecay - Decay forced to stop"));
	}
}

float URitualEnergyManagerComponent::GetTimeUntilDecay() const
{
	if (bIsDecaying)
	{
		return 0.0f; // Already decaying
	}

	float RemainingTime = DecayDelay - TimeSinceLastCombatAction;
	return FMath::Max(0.0f, RemainingTime);
}

void URitualEnergyManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!AbilitySystemComponent)
	{
		return;
	}

	// Track time since last combat action
	if (bIsInCombat)
	{
		TimeSinceLastCombatAction += DeltaTime;

		// Check if out-of-combat timer expired
		if (TimeSinceLastCombatAction >= DecayDelay)
		{
			bIsInCombat = false;

			// Start decay if not already active
			if (!bIsDecaying)
			{
				StartDecay();
			}
		}
	}
}

void URitualEnergyManagerComponent::StartDecay()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// Create decay effect spec
	// Note: If DecayEffectClass is not assigned, we'll create a runtime effect
	if (DecayEffectClass)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DecayEffectClass, 1.0f, EffectContext);
		if (SpecHandle.IsValid())
		{
			DecayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			bIsDecaying = true;

			UE_LOG(LogTemp, Log, TEXT("URitualEnergyManagerComponent::StartDecay - Ritual Energy decay started (Rate: %.1f/sec)"), DecayRatePerSecond);
		}
	}
	else
	{
		// Runtime effect creation (no Blueprint asset required)
		UGameplayEffect* DecayEffect = NewObject<UGameplayEffect>(GetOuter(), FName(TEXT("GE_RitualEnergyDecay_Runtime")));
		DecayEffect->DurationPolicy = EGameplayEffectDurationType::Infinite; // Persists until removed

		// Add modifier for RitualEnergy attribute
		FGameplayModifierInfo ModifierInfo;
		ModifierInfo.ModifierMagnitude = FScalableFloat(DecayRatePerSecond);
		ModifierInfo.ModifierOp = EGameplayModOp::Additive; // -1 per second
		ModifierInfo.Attribute = UAdventureAttributeSet::GetRitualEnergyAttribute();
		DecayEffect->Modifiers.Add(ModifierInfo);

		// Set period (apply every second)
		DecayEffect->Period = 1.0f;

		// Apply effect
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpec EffectSpec(DecayEffect, EffectContext, 1.0f);
		DecayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(EffectSpec);
		bIsDecaying = true;

		UE_LOG(LogTemp, Log, TEXT("URitualEnergyManagerComponent::StartDecay - Runtime decay effect created (Rate: %.1f/sec)"), DecayRatePerSecond);
	}
}

void URitualEnergyManagerComponent::StopDecay()
{
	if (!AbilitySystemComponent || !bIsDecaying)
	{
		return;
	}

	// Remove decay effect
	if (DecayEffectHandle.IsValid())
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(DecayEffectHandle);
		DecayEffectHandle.Invalidate();
	}

	bIsDecaying = false;
	UE_LOG(LogTemp, Log, TEXT("URitualEnergyManagerComponent::StopDecay - Ritual Energy decay stopped"));
}
