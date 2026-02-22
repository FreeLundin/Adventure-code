// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Player State (GAS)

#include "Core/AdventurePlayerState.h"
#include "AbilitySystemComponent.h"
#include "GAS/AdventureAttributeSet.h"
#include "GameplayEffect.h"

AAdventurePlayerState::AAdventurePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAdventureAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AAdventurePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAdventurePlayerState::InitializeAbilitySystem(AActor* AvatarActor, const TArray<TSubclassOf<UGameplayAbility>>& Abilities, TSubclassOf<UGameplayEffect> DefaultAttributesEffect)
{
	if (!AbilitySystemComponent || !AvatarActor)
	{
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(this, AvatarActor);

	if (GetLocalRole() == ROLE_Authority && !bAbilitiesGranted)
	{
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : Abilities)
		{
			if (*AbilityClass)
			{
				AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1));
			}
		}

		if (*DefaultAttributesEffect)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributesEffect, 1.0f, EffectContext);
			if (SpecHandle.IsValid())
			{
				AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}

		bAbilitiesGranted = true;
	}
}
