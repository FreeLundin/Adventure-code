// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Default Attributes Gameplay Effect

#include "GAS/AdventureDefaultAttributesEffect.h"
#include "GAS/AdventureAttributeSet.h"

UGE_AdventureDefaultAttributes::UGE_AdventureDefaultAttributes()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo HealthMod;
	HealthMod.Attribute = UAdventureAttributeSet::GetHealthAttribute();
	HealthMod.ModifierOp = EGameplayModOp::Override;
	HealthMod.ModifierMagnitude = FScalableFloat(100.0f);
	Modifiers.Add(HealthMod);

	FGameplayModifierInfo MaxHealthMod;
	MaxHealthMod.Attribute = UAdventureAttributeSet::GetMaxHealthAttribute();
	MaxHealthMod.ModifierOp = EGameplayModOp::Override;
	MaxHealthMod.ModifierMagnitude = FScalableFloat(100.0f);
	Modifiers.Add(MaxHealthMod);

	FGameplayModifierInfo StaminaMod;
	StaminaMod.Attribute = UAdventureAttributeSet::GetStaminaAttribute();
	StaminaMod.ModifierOp = EGameplayModOp::Override;
	StaminaMod.ModifierMagnitude = FScalableFloat(100.0f);
	Modifiers.Add(StaminaMod);

	FGameplayModifierInfo MaxStaminaMod;
	MaxStaminaMod.Attribute = UAdventureAttributeSet::GetMaxStaminaAttribute();
	MaxStaminaMod.ModifierOp = EGameplayModOp::Override;
	MaxStaminaMod.ModifierMagnitude = FScalableFloat(100.0f);
	Modifiers.Add(MaxStaminaMod);

	FGameplayModifierInfo RitualEnergyMod;
	RitualEnergyMod.Attribute = UAdventureAttributeSet::GetRitualEnergyAttribute();
	RitualEnergyMod.ModifierOp = EGameplayModOp::Override;
	RitualEnergyMod.ModifierMagnitude = FScalableFloat(100.0f);
	Modifiers.Add(RitualEnergyMod);

	FGameplayModifierInfo MaxRitualEnergyMod;
	MaxRitualEnergyMod.Attribute = UAdventureAttributeSet::GetMaxRitualEnergyAttribute();
	MaxRitualEnergyMod.ModifierOp = EGameplayModOp::Override;
	MaxRitualEnergyMod.ModifierMagnitude = FScalableFloat(100.0f);
	Modifiers.Add(MaxRitualEnergyMod);
}
