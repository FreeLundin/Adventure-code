// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Player State (GAS)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AdventurePlayerState.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
class UGameplayEffect;
class UAdventureAttributeSet;

UCLASS()
class ADVENTURE_API AAdventurePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAdventurePlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure, Category = "GAS")
	UAdventureAttributeSet* GetAttributeSet() const { return AttributeSet; }

	void InitializeAbilitySystem(AActor* AvatarActor, const TArray<TSubclassOf<UGameplayAbility>>& Abilities, TSubclassOf<UGameplayEffect> DefaultAttributesEffect);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAdventureAttributeSet> AttributeSet;

	bool bAbilitiesGranted = false;
};
