// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Attribute Set

#include "GAS/AdventureAttributeSet.h"
#include "Net/UnrealNetwork.h"

UAdventureAttributeSet::UAdventureAttributeSet()
{
	Health = 100.0f;
	MaxHealth = 100.0f;
	Stamina = 100.0f;
	MaxStamina = 100.0f;
	RitualEnergy = 100.0f;
	MaxRitualEnergy = 100.0f;
}

void UAdventureAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAdventureAttributeSet, Health, OldValue);
}

void UAdventureAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAdventureAttributeSet, MaxHealth, OldValue);
}

void UAdventureAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAdventureAttributeSet, Stamina, OldValue);
}

void UAdventureAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAdventureAttributeSet, MaxStamina, OldValue);
}

void UAdventureAttributeSet::OnRep_RitualEnergy(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAdventureAttributeSet, RitualEnergy, OldValue);
}

void UAdventureAttributeSet::OnRep_MaxRitualEnergy(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAdventureAttributeSet, MaxRitualEnergy, OldValue);
}

void UAdventureAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAdventureAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAdventureAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAdventureAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAdventureAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAdventureAttributeSet, RitualEnergy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAdventureAttributeSet, MaxRitualEnergy, COND_None, REPNOTIFY_Always);
}
