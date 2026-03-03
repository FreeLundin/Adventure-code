// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Player State (GAS)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
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

	virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintPure, Category = "GAS")
	UAdventureAttributeSet *GetAttributeSet() const { return AttributeSet; }

	void InitializeAbilitySystem(AActor *AvatarActor, const TArray<TSubclassOf<UGameplayAbility>> &Abilities, TSubclassOf<UGameplayEffect> DefaultAttributesEffect);

	/** Notify the state that a combat action occurred; resets decay timer. */
	void OnAbilityUsed();

	/** Overridden to advance combat timer and apply passive decay. */
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	/**
	 * Grant an ability via spec, returning the handle for later modification/removal.
	 * Stores the handle in a map keyed by class for convenience.
	 */
	UFUNCTION(BlueprintCallable, Category = "GAS")
	FGameplayAbilitySpecHandle GrantAbility(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level = 1, int32 InputID = -1);

	/** Remove a previously granted ability by handle. Safe to call with invalid handles. */
	UFUNCTION(BlueprintCallable, Category = "GAS")
	void RemoveAbility(const FGameplayAbilitySpecHandle Handle);

protected:
	/** Keep track of granted ability specs so we can query/modify/remove them. */
	UPROPERTY()
	TMap<TSubclassOf<UGameplayAbility>, FGameplayAbilitySpecHandle> GrantedAbilityHandles;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAdventureAttributeSet> AttributeSet;

	bool bAbilitiesGranted = false;

	// combat tracking for Ritual Energy decay
	float TimeSinceCombat = 0.0f;
};
