// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Type Definitions & Enums

#pragma once

#include "CoreMinimal.h"
#include "AdventureTypes.generated.h"

// ===== MOVEMENT ENUMS =====

/**
 * E_Gait
 * Character locomotion gait state (Walk, Run, Sprint)
 */
UENUM(BlueprintType)
enum E_Gait : uint8
{
	Walk UMETA(DisplayName = "Walk"),
	Run UMETA(DisplayName = "Run"),
	Sprint UMETA(DisplayName = "Sprint")
};

/**
 * E_AnalogStickBehavior
 * Input interpretation mode for analog sticks
 */
UENUM(BlueprintType)
enum E_AnalogStickBehavior : uint8
{
	Raw UMETA(DisplayName = "Raw"),
	Exponential UMETA(DisplayName = "Exponential"),
	Squared UMETA(DisplayName = "Squared"),
	Custom UMETA(DisplayName = "Custom")
};

/**
 * E_CameraStyle
 * Available camera perspectives
 */
UENUM(BlueprintType)
enum E_CameraStyle : uint8
{
	TopDown UMETA(DisplayName = "Top Down"),
	ThirdPerson UMETA(DisplayName = "Third Person"),
	FirstPerson UMETA(DisplayName = "First Person")
};

// ===== ANIMATION ENUMS =====

/**
 * E_FoleyEventSide
 * Which side/limb is generating the foley event (for stereo audio positioning)
 */
UENUM(BlueprintType)
enum class E_FoleyEventSide : uint8
{
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right"),
	Center UMETA(DisplayName = "Center")
};

/**
 * E_EarlyTransition_Condition
 * Conditions for early animation transition triggering
 */
UENUM(BlueprintType)
enum class E_EarlyTransition_Condition : uint8
{
	OnInput UMETA(DisplayName = "On Input"),
	OnStateChange UMETA(DisplayName = "On State Change"),
	OnTimer UMETA(DisplayName = "On Timer"),
	OnAnimProgress UMETA(DisplayName = "On Animation Progress")
};

/**
 * E_EarlyTransition_Destination
 * Target state after early transition
 */
UENUM(BlueprintType)
enum class E_EarlyTransition_Destination : uint8
{
	NextState UMETA(DisplayName = "Next State"),
	PreviousState UMETA(DisplayName = "Previous State"),
	SpecificState UMETA(DisplayName = "Specific State"),
	Idle UMETA(DisplayName = "Idle")
};

/**
 * E_TraversalBlendOutCondition
 * Conditions for blending out of traversal montages
 */
UENUM(BlueprintType)
enum class E_TraversalBlendOutCondition : uint8
{
	OnComplete UMETA(DisplayName = "On Complete"),
	OnInput UMETA(DisplayName = "On Input"),
	OnFail UMETA(DisplayName = "On Fail"),
	Immediate UMETA(DisplayName = "Immediate")
};

// ===== TRAVERSAL STRUCTURES =====

/**
 * FS_TraversalCheckInputs
 * Input parameters for traversal system queries
 */
USTRUCT(BlueprintType)
struct FS_TraversalCheckInputs
{
	GENERATED_BODY()

	/** Character's current world location */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector CharacterLocation = FVector::ZeroVector;

	/** Character's forward direction */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector CharacterForward = FVector::ForwardVector;

	/** Character's right direction */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector CharacterRight = FVector::RightVector;

	/** Desired traversal direction */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector TraversalDirection = FVector::ForwardVector;

	/** Character capsule radius */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CapsuleRadius = 42.0f;

	/** Character capsule half-height */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CapsuleHalfHeight = 88.0f;

	/** Whether to use debug visualization */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bUseDebugTraces = false;
};

/**
 * FS_TraversalCheckResult
 * Result of traversal system check query
 */
USTRUCT(BlueprintType)
struct FS_TraversalCheckResult
{
	GENERATED_BODY()

	/** Animation montage to play for this traversal */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<class UAnimMontage> SelectedMontage = nullptr;

	/** Target position for motion warping */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector TargetLocation = FVector::ZeroVector;

	/** Target rotation for motion warping */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FRotator TargetRotation = FRotator::ZeroRotator;

	/** Type of traversal action (vault, mantle, climb, etc.) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName TraversalType = NAME_None;

	/** Whether traversal check succeeded */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bSuccess = false;

	/** Distance to first ledge */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DistanceToFrontLedge = 0.0f;

	/** Distance to landing surface */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DistanceToBackFloor = 0.0f;
};

// ===== CHARACTER INPUT STATE =====

/**
 * FCharacterInputState
 * Player input state for replication
 */
USTRUCT(BlueprintType)
struct FCharacterInputState
{
	GENERATED_BODY()

	/** Normalized movement input (-1 to 1) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D MovementInput = FVector2D::ZeroVector;

	/** Normalized look input (-1 to 1) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D LookInput = FVector2D::ZeroVector;

	/** Whether sprint key is held */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bSprintInput = false;

	/** Whether jump/dodge key is pressed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bDodgeInput = false;

	/** Whether attack light key is pressed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bAttackLightInput = false;

	/** Whether attack heavy key is pressed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bAttackHeavyInput = false;

	/** Whether interact key is held */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bInteractionInput = false;

	/** Current camera style */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<E_CameraStyle> CameraStyle = E_CameraStyle::ThirdPerson;
};
