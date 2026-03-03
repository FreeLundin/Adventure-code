// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Player State (GAS)

#include "Core/AdventurePlayerState.h"
#include "AbilitySystemComponent.h"
#include "GAS/AdventureAttributeSet.h"
#include "GameplayEffect.h"

AAdventurePlayerState::AAdventurePlayerState()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAdventureAttributeSet>(TEXT("AttributeSet"));
}

void AAdventurePlayerState::BeginPlay()
{
	Super::BeginPlay();
	// ensure timer starts at 0
	TimeSinceCombat = 0.0f;
}

void AAdventurePlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() == ROLE_Authority && AbilitySystemComponent)
	{
		TimeSinceCombat += DeltaTime;
		if (TimeSinceCombat >= 8.0f)
		{
			float Current = AbilitySystemComponent->GetNumericAttribute(
				UAdventureAttributeSet::GetRitualEnergyAttribute());
			if (Current > 0.f)
			{
				FGameplayAttribute Attr = UAdventureAttributeSet::GetRitualEnergyAttribute();
				AbilitySystemComponent->ApplyModToAttribute(Attr, EGameplayModOp::Additive, -DeltaTime);
			}
		}
	}
}

void AAdventurePlayerState::OnAbilityUsed()
{
	TimeSinceCombat = 0.0f;
}

UAbilitySystemComponent *AAdventurePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAdventurePlayerState::InitializeAbilitySystem(AActor *AvatarActor, const TArray<TSubclassOf<UGameplayAbility>> &Abilities, TSubclassOf<UGameplayEffect> DefaultAttributesEffect)
{
	if (!AbilitySystemComponent || !AvatarActor)
	{
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(this, AvatarActor);

	if (GetLocalRole() == ROLE_Authority && !bAbilitiesGranted)
	{
		for (const TSubclassOf<UGameplayAbility> &AbilityClass : Abilities)
		{
			if (*AbilityClass)
			{
				// use helper so we track the handle
				GrantAbility(AbilityClass, 1, -1);
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

FGameplayAbilitySpecHandle AAdventurePlayerState::GrantAbility(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level, int32 InputID)
{
	FGameplayAbilitySpecHandle Handle;
	if (AbilitySystemComponent && *AbilityClass && HasAuthority())
	{
		FGameplayAbilitySpec Spec(AbilityClass, Level, InputID);
		Handle = AbilitySystemComponent->GiveAbility(Spec);
		if (Handle.IsValid())
		{
			GrantedAbilityHandles.Add(AbilityClass, Handle);
		}
	}
	return Handle;
}

void AAdventurePlayerState::RemoveAbility(const FGameplayAbilitySpecHandle Handle)
{
	if (AbilitySystemComponent && Handle.IsValid() && HasAuthority())
	{
		AbilitySystemComponent->ClearAbility(Handle);
		// remove from our map if present
		for (auto It = GrantedAbilityHandles.CreateIterator(); It; ++It)
		{
			if (It.Value() == Handle)
			{
				It.RemoveCurrent();
				break;
			}
		}
	}
}
