// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Mover-based Character Variant

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MoverSimulationTypes.h"
#include "MoverDataModelTypes.h"
#include "AbilitySystemInterface.h"
#include "GAS/AdventureGASInterface.h"
#include "CBP_AdventureCharacter_Mover.generated.h"

class UCameraComponent;
class UCapsuleComponent;
class UCharacterMoverComponent;
class USpringArmComponent;
class UInputAction;
class UAbilitySystemComponent;
class UGameplayAbility;
class UGameplayEffect;
struct FInputActionValue;

/**
 * ACBP_AdventureCharacter_Mover
 * 
 * Mover (Mover2) powered variant of the Adventure character.
 * Uses UMoverComponent/UCharacterMoverComponent for movement simulation
 * and exposes simple traversal hooks for Blueprint implementation.
 */
UCLASS(Blueprintable, BlueprintType)
class ADVENTURE_API ACBP_AdventureCharacter_Mover : public APawn, public IMoverInputProducerInterface, public IAbilitySystemInterface, public IAdventureGASInterface
{
	GENERATED_BODY()

public:
	ACBP_AdventureCharacter_Mover();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// IMoverInputProducerInterface
	virtual void ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult) override;

	// ===== GAS INITIALIZATION =====

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void InitializeAbilitySystem() override;
	virtual const TArray<TSubclassOf<UGameplayAbility>>& GetDefaultAbilities() const override { return DefaultAbilities; }
	virtual TSubclassOf<UGameplayEffect> GetDefaultAttributesEffect() const override { return DefaultAttributesEffect; }

	// ===== INPUT ROUTING =====

	/** Set movement input (typically from PlayerController). Range -1..1 per axis. */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetMoveInputVector(const FVector2D& MoveInput);

	/** Set look input (typically from PlayerController). Range -1..1 per axis. */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetLookInputVector(const FVector2D& LookInput);

	/** Set jump pressed state. */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetJumpPressed(bool bPressed);

	/** Set sprint pressed state (reserved for custom movement logic). */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetSprintPressed(bool bPressed);

	/** Set crouch pressed state (reserved for custom movement logic). */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetCrouchPressed(bool bPressed);

	// ===== TRAVERSAL HOOKS =====

	UFUNCTION(BlueprintCallable, Category = "Traversal")
	void RequestTraversalVault();

	UFUNCTION(BlueprintCallable, Category = "Traversal")
	void RequestTraversalMantle();

	UFUNCTION(BlueprintCallable, Category = "Traversal")
	void RequestTraversalClimb();

	// make these native so C++ can supply a default implementation
	UFUNCTION(BlueprintNativeEvent, Category = "Traversal")
	void OnTraversalRequestVault();

	UFUNCTION(BlueprintNativeEvent, Category = "Traversal")
	void OnTraversalRequestMantle();

	UFUNCTION(BlueprintNativeEvent, Category = "Traversal")
	void OnTraversalRequestClimb();

	// ===== ACCESSORS =====

	UFUNCTION(BlueprintPure, Category = "Mover")
	UCharacterMoverComponent* GetMoverComponent() const { return CharacterMoverComponent; }

	// ===== GAS CONFIGURATION =====

	/** Gameplay Ability System component (owned by PlayerState) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/** Default abilities granted to this pawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	/** Optional default attribute effect applied on spawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> DefaultAttributesEffect;

protected:
	// Optional native input production extension point
	virtual void OnProduceInput(float DeltaMs, FMoverInputCmdContext& InputCmdResult);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Produce Input", meta = (ScriptName = "OnProduceInput"))
	FMoverInputCmdContext OnProduceInputInBlueprint(float DeltaMs, FMoverInputCmdContext InputCmd);

	// ===== COMPONENTS =====

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCharacterMoverComponent> CharacterMoverComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> Camera;

	// ===== OPTIONAL ENHANCED INPUT (BP ASSIGNED) =====

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> SprintInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> CrouchInputAction;

	// ===== INPUT STATE =====

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float LookSensitivity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	bool bInvertLookY = false;

private:
	FVector CachedMoveInputIntent = FVector::ZeroVector;
	FVector2D CachedLookInput = FVector2D::ZeroVector;

	bool bJumpJustPressed = false;
	bool bJumpHeld = false;
	bool bSprintHeld = false;
	bool bCrouchHeld = false;

	void OnMoveTriggered(const FInputActionValue& Value);
	void OnMoveCompleted(const FInputActionValue& Value);
	void OnLookTriggered(const FInputActionValue& Value);
	void OnLookCompleted(const FInputActionValue& Value);
	void OnJumpStarted(const FInputActionValue& Value);
	void OnJumpReleased(const FInputActionValue& Value);
	void OnSprintStarted(const FInputActionValue& Value);
	void OnSprintReleased(const FInputActionValue& Value);
	void OnCrouchStarted(const FInputActionValue& Value);
	void OnCrouchReleased(const FInputActionValue& Value);
};
