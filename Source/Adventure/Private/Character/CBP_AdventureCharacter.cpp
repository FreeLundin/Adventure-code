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
#include "Net/UnrealNetwork.h" // for replication macros

#include "Components/CapsuleComponent.h" // needed for capsule access

ACBP_AdventureCharacter::ACBP_AdventureCharacter()
	: CachedCharMovement(nullptr), CachedAdventureController(nullptr)
{
	AbilitySystemComponent = nullptr;

	// Set up default character properties
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// initialize example variable
	ExampleDebugFloat = 0.0f;

	// Configure character movement
	if (UCharacterMovementComponent *CharMovement = GetCharacterMovement())
	{
		CharMovement->bOrientRotationToMovement = true;
		CharMovement->MaxWalkSpeed = 600.0f;
		CharMovement->MaxAcceleration = 2048.0f;
		CachedCharMovement = CharMovement;
	}

	// Start with tick disabled for proxies; owner will enable as needed
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	// Initialize movement state (defaults will be used from header declarations)
	// Gait, speeds, and other properties are initialized via UPROPERTY defaults

	DefaultAbilities = {
		UGA_AdventureSprint::StaticClass(),
		UGA_AdventureDodge::StaticClass(),
		UGA_AdventureTraversal::StaticClass(),
		UGA_AdventureInteract::StaticClass(),
		UGA_AdventureLightAttack::StaticClass(),
		UGA_AdventureHeavyAttack::StaticClass()};

	DefaultAttributesEffect = UGE_AdventureDefaultAttributes::StaticClass();
}

void ACBP_AdventureCharacter::PossessedBy(AController *NewController)
{
	Super::PossessedBy(NewController);
	InitializeAbilitySystem();

	// once possessed, setup input bindings and cache references
	SetupInput();
	Super::OnRep_PlayerState();
	InitializeAbilitySystem();
}

UAbilitySystemComponent *ACBP_AdventureCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACBP_AdventureCharacter::InitializeAbilitySystem()
{
	AAdventurePlayerState *AdventurePS = GetPlayerState<AAdventurePlayerState>();
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
	// apply a small dead zone to reduce stick drift
	const float DeadZone = 0.1f;
	float X = FMath::Abs(Input.X) < DeadZone ? 0.0f : Input.X;
	float Y = FMath::Abs(Input.Y) < DeadZone ? 0.0f : Input.Y;

	// optional: further scale input (e.g. square for sensitivity curve)
	X = FMath::Sign(X) * FMath::Square(FMath::Abs(X));
	Y = FMath::Sign(Y) * FMath::Square(FMath::Abs(Y));

	return FVector2D(X, Y);
}

void ACBP_AdventureCharacter::SetupInput()
{
	// cache movement component in case it wasn't cached in constructor
	if (!CachedCharMovement)
	{
		CachedCharMovement = GetCharacterMovement();
	}

	// cache controller as our custom AdventureController type
	if (Controller && !CachedAdventureController)
	{
		CachedAdventureController = Cast<APC_AdventureController>(Controller);
	}

	// Allow this pawn to tick if we are locally controlled (input will arrive)
	if (IsLocallyControlled())
	{
		SetActorTickEnabled(true);
	}

	// note: actual binding of input actions is performed on the controller
	// via the enhanced input setup.  Character-specific action handlers can
	// be exposed as BlueprintImplementableEvents if needed.
}
void ACBP_AdventureCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_WITH_PARAMS(ACBP_AdventureCharacter, TraversalResult, COND_SimulatedOnly);
	DOREPLIFETIME_WITH_PARAMS(ACBP_AdventureCharacter, CharacterInputState, COND_SkipOwner);
}

bool ACBP_AdventureCharacter::HasMovementInputVector()
{
	FVector2D Input(GetLastMovementInputVector());
	// treat very small values as zero
	return !Input.IsNearlyZero(0.05f);
}

bool ACBP_AdventureCharacter::CanSprint()
{
	// basic preconditions: must be moving and not crouched
	if (!HasMovementInputVector())
	{
		return false;
	}
	if (CachedCharMovement && CachedCharMovement->IsCrouching())
	{
		return false;
	}

	// stamina or ability checks could go here, e.g. query ASC for a sprint tag
	// if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(...))
	//     return false;

	void ACBP_AdventureCharacter::UpdateMovement_PreCMC()
	{
		// Guard: Only process on owner (client-side prediction)
		if (!IsLocallyControlled())
		{
			return;
		}

		// Get character movement component (cached if available)
		UCharacterMovementComponent *CharMC = CachedCharMovement ? CachedCharMovement : GetCharacterMovement();
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
		if (CanSprint())
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
	// UE_LOG(LogTemp, Warning, TEXT("UpdateMovement_PreCMC: Input=%.2f,%.2f Velocity=%.1f,%.1f"),
	//	InputDirection.X, InputDirection.Y, CharMC->Velocity.X, CharMC->Velocity.Y);
}

// OnCustomAction_Implementation is provided by the UFUNCTION macro via GENERATED_BODY()
// and should be implemented only once in this cpp file if needed. Keep this file
// with no duplicate definitions to avoid linkage issues.

// ------------------------------------------------------------------
// Optimization subclass implementation
// ------------------------------------------------------------------

void ACBP_AdventureCharacter_CMC::Tick(float DeltaSeconds)
{
	// call parent to preserve normal behavior (abilities, animation updates, etc.)
	Super::Tick(DeltaSeconds);

	// compute cached values once per frame then feed into PreCMC hooks
	ComputeCachedMovementValues();

	// if there's no input, skip the expensive updates entirely
	if (!IsLocallyControlled() || CachedMovementInput.IsNearlyZero(0.05f))
	{
		return;
	}

	UpdateMovement_PreCMC();
	UpdateRotation_PreCMC();
	if (StrafeSpeedMapCurve)
	{
		// example: assume X axis corresponds to yaw difference between velocity and forward
		float Angle = 0.0f;
		if (!CachedMovementInput.IsNearlyZero())
		{
			FVector Input3D(CachedMovementInput, 0.0f);
			Angle = FMath::Abs(FMath::Acos(FVector::DotProduct(Input3D.GetSafeNormal(), GetActorForwardVector())));
			Angle = FMath::Clamp(Angle / PI, 0.0f, 1.0f);
		}
		CachedStrafeSpeedMap = StrafeSpeedMapCurve->GetFloatValue(Angle);
	}
}

void ACBP_AdventureCharacter_CMC::UpdateMovement_PreCMC()
{
	// use cached values to avoid calling into blueprint nodes repeatedly
	FVector2D Input = CachedMovementInput;
	if (Input.IsNearlyZero())
	{
		return;
	}

	// simplified logic here; most of the blueprint graph is now executed in C++
	UCharacterMovementComponent *CharMC = GetCharacterMovement();
	if (!CharMC)
		return;

	// determine speed vector based on gait and cached strafe map
	double MaxSpeed = CalculateMaxSpeed(CachedStrafeSpeedMap);
	FVector Direction3D(Input, 0.0f);
	Direction3D = Direction3D.GetClampedToMaxSize(1.0f);

	FVector DesiredVel = (GetActorForwardVector() * Direction3D.X +
						  GetActorRightVector() * Direction3D.Y) *
						 MaxSpeed;

	CharMC->Velocity.X = DesiredVel.X;
	CharMC->Velocity.Y = DesiredVel.Y;
}

void ACBP_AdventureCharacter_CMC::UpdateRotation_PreCMC()
{
	// rely on the base implementation but use cached input to minimize cost
	FVector2D Input = CachedMovementInput;
	if (Input.IsNearlyZero())
		return;

	// derive world direction
	FVector WorldDir(Input, 0.0f);
	WorldDir.Normalize();
	FRotator DesiredRot = WorldDir.Rotation();
	FRotator CurrRot = GetActorRotation();
	FRotator NewRot = FMath::RInterpTo(CurrRot, DesiredRot, GetWorld()->DeltaTimeSeconds, 15.0f);
	SetActorRotation(NewRot);
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

void ACBP_AdventureCharacter::SetupCamera(APlayerController *PlayerController)
{
	// TODO: Initialize camera components and attach to character
	// Set initial camera mode based on CameraStyle
}

FS_TraversalCheckInputs ACBP_AdventureCharacter::GetTraversalCheckInputs(FVector Direction)
{
	FS_TraversalCheckInputs Inputs;
	Inputs.CharacterLocation = GetActorLocation();
	Inputs.CharacterForward = GetActorForwardVector();
	Inputs.CharacterRight = GetActorRightVector();
	Inputs.TraversalDirection = Direction.IsNearlyZero() ? Inputs.CharacterForward : Direction.GetSafeNormal();

	if (UCapsuleComponent *Capsule = GetCapsuleComponent())
	{
		Inputs.CapsuleRadius = Capsule->GetScaledCapsuleRadius();
		Inputs.CapsuleHalfHeight = Capsule->GetScaledCapsuleHalfHeight();
	}

	Inputs.bUseDebugTraces = false;
	return Inputs;
}

void ACBP_AdventureCharacter::TryTraversalAction(
	FS_TraversalCheckInputs Inputs,
	bool bEnableDebugDraw,
	bool &TraversalCheckFailed,
	bool &MontageSelectionFailed,
	FS_TraversalCheckResult TraversalCheckResult,
	FVector ActorLocation,
	float CapsuleRadius,
	float CapsuleHalfHeight,
	FVector HasRoomCheck_FrontLedgeLocation,
	FVector HasRoomCheck_BackLedgeLocation,
	FHitResult TopSweepResult,
	int32 DrawDebugLevel,
	double DrawDebugDuration,
	TArray<UAnimMontage *> ValidMontages)
{
	UE_LOG(LogTemp, Log, TEXT("[Traversal] TryTraversalAction called (dir=%s)"), *Inputs.TraversalDirection.ToString());

	// very basic stub: always fail so caller knows nothing happened
	TraversalCheckFailed = false;
	MontageSelectionFailed = false;
	TraversalCheckResult.bSuccess = false;
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
	UE_LOG(LogTemp, Log, TEXT("[Traversal] OnTraversalStart for %s"), *GetName());
	// disable collision during traversal
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACBP_AdventureCharacter::OnTraversalEnd()
{
	UE_LOG(LogTemp, Log, TEXT("[Traversal] OnTraversalEnd for %s"), *GetName());
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ACBP_AdventureCharacter::OnRep_TraversalResult()
{
	UE_LOG(LogTemp, Verbose, TEXT("[Traversal] OnRep_TraversalResult for %s"), *GetName());
	// blueprint or montage code handled by derived classes
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
