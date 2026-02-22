// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Attribute Set

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AdventureAttributeSet.generated.h"

#define ADVENTURE_ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class ADVENTURE_API UAdventureAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAdventureAttributeSet();

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attributes")
	FGameplayAttributeData Health;
	ADVENTURE_ATTRIBUTE_ACCESSORS(UAdventureAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ADVENTURE_ATTRIBUTE_ACCESSORS(UAdventureAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Attributes")
	FGameplayAttributeData Stamina;
	ADVENTURE_ATTRIBUTE_ACCESSORS(UAdventureAttributeSet, Stamina)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "Attributes")
	FGameplayAttributeData MaxStamina;
	ADVENTURE_ATTRIBUTE_ACCESSORS(UAdventureAttributeSet, MaxStamina)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_RitualEnergy, Category = "Attributes")
	FGameplayAttributeData RitualEnergy;
	ADVENTURE_ATTRIBUTE_ACCESSORS(UAdventureAttributeSet, RitualEnergy)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxRitualEnergy, Category = "Attributes")
	FGameplayAttributeData MaxRitualEnergy;
	ADVENTURE_ATTRIBUTE_ACCESSORS(UAdventureAttributeSet, MaxRitualEnergy)

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_RitualEnergy(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_MaxRitualEnergy(const FGameplayAttributeData& OldValue) const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

#undef ADVENTURE_ATTRIBUTE_ACCESSORS
