// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Core Character Implementation

#include "Character/CBP_AdventureCharacter.h"
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Core/AdventurePlayerState.h"
#include "Curves/CurveFloat.h"
#include "GAS/AdventureDefaultAttributesEffect.h"
#include "GAS/AdventureGameplayAbility.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayTagsManager.h"

ACBP_AdventureCharacter::ACBP_AdventureCharacter()
{
	AbilitySystemComponent = nullptr;

	// Set up default character properties
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	if (UCharacterMovementComponent* CharMovement = GetCharacterMovement())
	{
		CharMovement->bOrientRotationToMovement = true;
		CharMovement->MaxWalkSpeed = 600.0f;
		CharMovement->MaxAcceleration = 2048.0f;
	}

	// Initialize movement state (defaults will be used from header declarations)
	// Gait, speeds, and other properties are initialized via UPROPERTY defaults

	DefaultAbilities = {
		UGA_AdventureSprint::StaticClass(),
		UGA_AdventureDodge::StaticClass(),
		UGA_AdventureTraversal::StaticClass(),
		UGA_AdventureInteract::StaticClass()
	};

	DefaultAttributesEffect = UGE_AdventureDefaultAttributes::StaticClass();
}

void ACBP_AdventureCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitializeAbilitySystem();
}

void ACBP_AdventureCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitializeAbilitySystem();
}

UAbilitySystemComponent* ACBP_AdventureCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACBP_AdventureCharacter::InitializeAbilitySystem()
{
	AAdventurePlayerState* AdventurePS = GetPlayerState<AAdventurePlayerState>();
	if (!AdventurePS)
	{
		return;
	}

	AbilitySystemComponent = AdventurePS->GetAbilitySystemComponent();
	if (!AbilitySystemComponent)
	{
		return;
	}

	AdventurePS->InitializeAbilitySystem(this, DefaultAbilities, DefaultAttributesEffect);
}

FVector2D ACBP_AdventureCharacter::GetMovementInputScaleValue(FVector2D Input)
{
	// TODO: Implement dead-zone and sensitivity scaling
	return Input;
}

void ACBP_AdventureCharacter::SetupInput()
{
	// TODO: Bind input actions to character functions
	// Called by PlayerController to set up input mappings
}

bool ACBP_AdventureCharacter::HasMovementInputVector()
{
	// TODO: Query current movement input state
	return false;
}

bool ACBP_AdventureCharacter::CanSprint()
{
	// TODO: Check stamina and movement prerequisites for sprint
	return true;
}

void ACBP_AdventureCharacter::UpdateMovement_PreCMC()
{
	// Guard: Only process on owner (client-side prediction)
	if (!IsLocallyControlled())
	{
		return;
	}

	// Get character movement component
	UCharacterMovementComponent* CharMC = GetCharacterMovement();
	if (!CharMC)
	{
		return;
	}

	// Get current movement input vector (WASD or analog stick)
	// This is consumed from the input buffer by the character movement system
	FVector InputDirection = GetLastMovementInputVector();
	
	// Guard: No input
	if (InputDirection.IsZero())
	{
		// TODO: Decelerate smoothly if moving
		return;
	}

	// Determine desired gait based on input and sprint state
	// For now: Use simple speed mapping
	//   - Walk: Normal speed (from WalkSpeeds)
	//   - Run: Increased speed (from RunSpeeds)
	//   - Sprint: Maximum speed (from SprintSpeeds) if stamina available
	
	FVector MaxSpeed = WalkSpeeds; // Default to walk speeds
	
	// TODO: Check if sprinting (check GAS ability state or sprint input)
	// For now, if moving, use run speeds
	if (HasMovementInputVector())
	{
		MaxSpeed = RunSpeeds;
	}

	// Apply movement to character movement component
	// Convert input direction to world space if needed
	FVector WorldDirection = InputDirection;
	
	// Clamp magnitude to 1.0 (input normalization)
	WorldDirection = WorldDirection.GetClampedToMaxSize(1.0f);

	// Apply velocity based on desired direction and max speed
	// Use forward speed for forward movement, strafe for left/right, backward for back
	float ForwardSpeed = FVector::DotProduct(WorldDirection, GetActorForwardVector()) * MaxSpeed.X;
	float StrafeSpeed = FVector::DotProduct(WorldDirection, GetActorRightVector()) * MaxSpeed.Y;

	FVector DesiredVelocity = (GetActorForwardVector() * ForwardSpeed) + (GetActorRightVector() * StrafeSpeed);
	
	// Apply to character movement velocity
	CharMC->Velocity.X = DesiredVelocity.X;
	CharMC->Velocity.Y = DesiredVelocity.Y;
	// Preserve Z velocity (gravity/jump)

	// Log movement for debugging
	//UE_LOG(LogTemp, Warning, TEXT("UpdateMovement_PreCMC: Input=%.2f,%.2f Velocity=%.1f,%.1f"), 
	//	InputDirection.X, InputDirection.Y, CharMC->Velocity.X, CharMC->Velocity.Y);
}

void ACBP_AdventureCharacter::UpdateRotation_PreCMC()
{
	// Guard: Only process on owner
	if (!IsLocallyControlled())
	{
		return;
	}

	// TODO: Get look input (mouse/analog stick) from PlayerController camera
	// For now, rotate character to face movement direction
	
	FVector InputDirection = GetLastMovementInputVector();
	
	// If no input, face current direction (no rotation update needed)
	if (InputDirection.IsZero())
	{
		return;
	}

	// Normalize input direction
	InputDirection = InputDirection.GetSafeNormal();

	// Calculate desired rotation from input direction
	// In third-person, character should face the direction of input
	FRotator DesiredRotation = InputDirection.Rotation();

	// Apply rotation with smooth interpolation
	FRotator CurrentRotation = GetActorRotation();
	const float RotationSpeed = 10.0f; // Degrees per...tick? (will need tuning)
	
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, DesiredRotation, GetWorld()->DeltaTimeSeconds, RotationSpeed);
	SetActorRotation(NewRotation);

	// TODO: Clamp rotation based on camera style:
	//   - Top-Down: Allow full 360 rotation
	//   - Third-Person: Rotate toward input, camera focus
	//   - First-Person: Use camera direction directly
}

TEnumAsByte<E_Gait> ACBP_AdventureCharacter::GetDesiredGait(bool FullMovementInput)
{
	// Determine gait based on movement input and character state
	// TODO: Check sprint ability active status from GAS
	// For now: Walk if no input, Run if input present
	
	if (!FullMovementInput)
	{
		return E_Gait::Walk; // Default walk
	}

	// TODO: Check if sprint ability is active
	// For now, always run when moving
	return E_Gait::Run;
}

double ACBP_AdventureCharacter::CalculateMaxSpeed(float StrafeSpeedMap)
{
	// Calculate max speed based on current gait and strafe angle
	// StrafeSpeedMap comes from animating the character's strafe angle (0-1 curve)
	// This determines speed reduction when moving sideways or backward
	
	FVector MaxSpeeds = WalkSpeeds; // Default
	
	switch (Gait)
	{
	case E_Gait::Walk:
		MaxSpeeds = WalkSpeeds;
		break;
	case E_Gait::Run:
		MaxSpeeds = RunSpeeds;
		break;
	case E_Gait::Sprint:
		MaxSpeeds = SprintSpeeds;
		break;
	default:
		MaxSpeeds = WalkSpeeds;
	}
	
	// Forward movement (strafe map = 0) uses X
	// Strafe movement (strafe map = 0.5) uses Y
	// Backward (strafe map = 1.0) uses Z
	float FinalMaxSpeed = FMath::Lerp(MaxSpeeds.X, MaxSpeeds.Z, StrafeSpeedMap);
	
	return FinalMaxSpeed;
}

double ACBP_AdventureCharacter::CalculateMaxAcceleration(float StrafeSpeedMap)
{
	// Acceleration varies by gait
	// Higher acceleration = snappier response to input
	switch (Gait)
	{
	case E_Gait::Walk:
		return 1024.0f; // Slow, controlled acceleration
	case E_Gait::Run:
		return 2048.0f; // Standard acceleration
	case E_Gait::Sprint:
		return 4096.0f; // Fast acceleration for sprinting
	default:
		return 2048.0f;
	}
}

double ACBP_AdventureCharacter::CalculateBrakingDeceleration(float StrafeSpeedMap)
{
	// Braking deceleration when input is released
	// Higher values = quicker stop
	switch (Gait)
	{
	case E_Gait::Walk:
		return 2048.0f; // Normal braking
	case E_Gait::Run:
		return 2048.0f; // Same as walk
	case E_Gait::Sprint:
		return 1024.0f; // Slower braking from sprint (momentum)
	default:
		return 2048.0f;
	}
}

double ACBP_AdventureCharacter::CalculateBrakingFriction(float StrafeSpeedMap)
{
	// TODO: Calculate braking friction coefficient
	return 8.0f;
}

double ACBP_AdventureCharacter::CalculateGroundFriction(float StrafeSpeedMap)
{
	// TODO: Calculate ground friction coefficient
	return 8.0f;
}

double ACBP_AdventureCharacter::CalculateMaxCrouchSpeed(float StrafeSpeedMap)
{
	// TODO: Calculate max crouch speed based on strafe angle
	return CrouchSpeeds.X;
}

void ACBP_AdventureCharacter::SetupCamera(APlayerController* PlayerController)
{
	// TODO: Initialize camera components and attach to character
	// Set initial camera mode based on CameraStyle
}

FS_TraversalCheckInputs ACBP_AdventureCharacter::GetTraversalCheckInputs(FVector Direction)
{
	// TODO: Build traversal check input struct from character state and direction
	FS_TraversalCheckInputs Inputs;
	return Inputs;
}

void ACBP_AdventureCharacter::TryTraversalAction(
	FS_TraversalCheckInputs Inputs,
	bool bEnableDebugDraw,
	bool& TraversalCheckFailed,
	bool& MontageSelectionFailed,
	FS_TraversalCheckResult TraversalCheckResult,
	FVector ActorLocation,
	float CapsuleRadius,
	float CapsuleHalfHeight,
	FVector HasRoomCheck_FrontLedgeLocation,
	FVector HasRoomCheck_BackLedgeLocation,
	FHitResult TopSweepResult,
	int32 DrawDebugLevel,
	double DrawDebugDuration,
	TArray<UAnimMontage*> ValidMontages)
{
	// TODO: Perform collision checks, select animation montage, trigger traversal
	TraversalCheckFailed = true;
	MontageSelectionFailed = true;
}

void ACBP_AdventureCharacter::UpdateWarpTargets(double AnimatedDistanceFromFrontLedgeToBackLedge, double AnimatedDistanceFromFrontLedgeToBackFloor)
{
	// TODO: Update motion warp targets during traversal animations
}

void ACBP_AdventureCharacter::Traversal_ServerImplementation(FS_TraversalCheckResult TraversalRep)
{
	// TODO: Server-side traversal action implementation
}

void ACBP_AdventureCharacter::OnTraversalStart()
{
	// TODO: Called when traversal montage begins
	// Disable collision, cache velocity, trigger audio/VFX
}

void ACBP_AdventureCharacter::OnTraversalEnd()
{
	// TODO: Called when traversal montage ends
	// Re-enable collision, restore control, transition to normal locomotion
}

void ACBP_AdventureCharacter::OnRep_TraversalResult()
{
	// TODO: Replication callback for traversal result updates
	// Sync animation on remote clients
}

void ACBP_AdventureCharacter::UpdatedMovementSimulated(FVector OldVelocity, bool IsMovingOnGround)
{
	// TODO: Update animation state for simulated movement
	// Trigger landing effects when transitioning to ground
}

void ACBP_AdventureCharacter::PlayAudioEvent(FGameplayTag Value, float VolumeMultiplier, float PitchMultiplier)
{
	// TODO: Play audio event with optional volume/pitch modulation
}

void ACBP_AdventureCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// TODO: Register replicated properties for network synchronization
	// Example: DOREPLIFETIME(ACBP_AdventureCharacter, CurrentInputState);
}
