// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - GAS Interface

#pragma once

#include "UObject/Interface.h"
#include "Templates/SubclassOf.h"
#include "AdventureGASInterface.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UINTERFACE(MinimalAPI, BlueprintType)
class UAdventureGASInterface : public UInterface
{
	GENERATED_BODY()
};

class IAdventureGASInterface
{
	GENERATED_BODY()

public:
	virtual void InitializeAbilitySystem() = 0;
	virtual const TArray<TSubclassOf<UGameplayAbility>>& GetDefaultAbilities() const = 0;
	virtual TSubclassOf<UGameplayEffect> GetDefaultAttributesEffect() const = 0;
};
