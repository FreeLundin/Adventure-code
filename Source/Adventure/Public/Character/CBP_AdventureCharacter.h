// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Advanced Locomotion & Traversal Character System

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemInterface.h"
#include "GAS/AdventureGASInterface.h"
#include "Core/AdventureTypes.h"
#include "Core/AdventureMacros.h" // project-wide reflection helpers
#include "CBP_AdventureCharacter.generated.h"

class APlayerController;
class APC_AdventureController;
class UAbilitySystemComponent;
class UGameplayAbility;
class UGameplayEffect;
class UPhysicalAnimationComponent;

/**
 * ACBP_AdventureCharacter
 * 
 * Advanced locomotion and traversal character for the Adventure prototype (Phase 1+).
 * This class provides a foundation for kinetic, parkour-like movement combining:
 *   - Multi-perspective camera system (TopDown/ThirdPerson/FirstPerson)
 *   - Fluid locomotion (walk, sprint, dodge) with curve-based speed mapping
 *   - Advanced traversal mechanics (vault, mantle, climb, rope swing, ziplines, wall runs)
 *   - Environmental interaction (grapple hooks, pole climbing, pushing/pulling, narrow paths)
 *   - Water-based traversal (swimming)
 *   - Motion warping for smooth transitions and animation-to-world synchronization
 *   - Audio event system for footsteps and action feedback
 * 
 * Integration Points:
 *   - Designed to work with GAS (Gameplay Ability System) for abilities and effects
 *   - Accepts RitualEnergy AttributeSet for ability resource management
 *   - Supports networked replication for multiplayer-ready architecture
 *   - Uses Enhanced Input System for flexible input mapping
 * 
 * Animation System:
 *   - Supports both montage-based and motion-matching animation workflows
 *   - Exposes movement state (gait, landing velocity, traversal status) to AnimBlueprint
 *   - Provides motion warping for precise character positioning during animations
 * 
 * Usage:
 *   1. Reparent or extend this class in Blueprint as CBP_AdventureCharacter
 *   2. Configure camera styles and traversal components in Blueprint defaults
 *   3. Bind input actions via SetupInput() and input mapping context
 *   4. Enable desired traversal systems (climbing, swimming, ziplining, etc.)
 *   5. Integrate with PlayerController and HUD for feedback
 * 
 * @see UGameplayCameraComponent for camera behavior
 * @see FS_TraversalCheckInputs, FS_TraversalCheckResult for traversal query system
 * @see FS_CharacterInputState for input state replication
 */
UCLASS(Blueprintable, BlueprintType)
class ADVENTURE_API ACBP_AdventureCharacter : public ACharacter, public IAbilitySystemInterface, public IAdventureGASInterface
{
	GENERATED_BODY()

public:
	// ===== CONSTRUCTION & INITIALIZATION =====

	/** Constructor */
	ACBP_AdventureCharacter();

    // BlueprintNativeEvent that can be overridden in either C++ or BP
    ADV_NATIVE_EVENT()
    void OnCustomAction();


	// ===== GAS INITIALIZATION =====

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void InitializeAbilitySystem() override;
	virtual const TArray<TSubclassOf<UGameplayAbility>>& GetDefaultAbilities() const override { return DefaultAbilities; }
	virtual TSubclassOf<UGameplayEffect> GetDefaultAttributesEffect() const override { return DefaultAttributesEffect; }

	// ===== INPUT & MOVEMENT QUERIES =====

	/**
	 * GetMovementInputScaleValue
	 * 
	 * Processes raw analog stick input (Range: -1 to 1) and applies sensitivity/dead-zone scaling.
	 * Used to normalize and smooth input vectors before passing to movement system.
	 * 
	 * @param Input		Raw input from analog stick (typically -1 to 1 on each axis)
	 * @return			Scaled/normalized movement input vector
	 */
	UFUNCTION(BlueprintPure, Category = "Input")
	FVector2D GetMovementInputScaleValue(FVector2D Input);

	/**
	 * SetupInput
	 * 
	 * Initializes input bindings and action mappings for the character.
	 * Called during PlayerController setup to establish input context.
	 * Binds movement, camera toggle, abilities, and interact actions.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetupInput();

	/**
	 * HasMovementInputVector
	 * 
	 * Queries whether the character currently has non-zero movement input.
	 * Used for state machine transitions and ability activation gating.
	 * 
	 * @return			True if player is providing directional input, false otherwise
	 */
	UFUNCTION(BlueprintPure, Category = "Movement")
	bool HasMovementInputVector();

	/**
	 * CanSprint
	 * 
	 * Evaluates whether character is able to enter sprint state.
	 * Checks stamina (if applicable via GAS), input, and movement prerequisites.
	 * 
	 * @return			True if sprint conditions are met, false if blocked or resource throttled
	 */
	UFUNCTION(BlueprintPure, Category = "Movement")
	bool CanSprint();

	// ===== CHARACTER MOVEMENT & ROTATION =====

	/**
	 * UpdateMovement_PreCMC
	 * 
	 * Pre-character movement component update hook called before CMC tick.
	 * Applies input-based acceleration, velocity, and state updates.
	 * Typically called from AnimBlueprint or PlayerController input processing.
	 * 
	 * @see UpdateRotation_PreCMC for simultaneous rotation updates
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void UpdateMovement_PreCMC();

	/**
	 * UpdateRotation_PreCMC
	 * 
	 * Pre-character movement component rotation update called before CMC tick.
	 * Rotates character toward look direction based on camera style and input.
	 * Handles smooth rotation interpolation and strafing angle calculation.
	 * 
	 * @see UpdateMovement_PreCMC for simultaneous movement updates
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void UpdateRotation_PreCMC();

	/**
	 * GetDesiredGait
	 * 
	 * Determines target movement gait (Walk/Run/Sprint) based on input and character state.
	 * Uses curve-based speed mapping and input magnitude to smooth gait transitions.
	 * 
	 * @param FullMovementInput	If true, player has full analog stick deflection; false means partial input
	 * @return					Desired gait enum (Walk, Run, or Sprint)
	 */
	UFUNCTION(BlueprintPure, Category = "Movement")
	TEnumAsByte<E_Gait> GetDesiredGait(bool FullMovementInput);

	// ===== GAS CONFIGURATION =====

	/** Gameplay Ability System component (owned by PlayerState) */
	// ASC component is owned by playerstate; exposed read-only for convenience
	ADV_PROP_RO()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/** Default abilities granted to this character */
	ADV_PROP_RO(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	/** Optional default attribute effect applied on spawn */
	ADV_PROP_RO(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DefaultAttributesEffect;

	// ===== MOVEMENT PARAMETER CALCULATION =====

	/**
	 * CalculateMaxSpeed
	 * 
	 * Queries the movement system's desired maximum velocity for current gait and strafe direction.
	 * References StrafeSpeedMapCurve for smooth speed ramping.
	 * Gait-specific speeds (Walk/Run/Sprint) are applied based on current state.
	 * 
	 * @param StrafeSpeedMap	Normalized strafe angle input (0-1) for curve lookup
	 * @return				Maximum speed in cm/s for current movement state
	 */
	UFUNCTION(BlueprintPure, Category = "Movement")
	double CalculateMaxSpeed(float StrafeSpeedMap);

	/**
	 * CalculateMaxAcceleration
	 * 
	 * Computes character acceleration magnitude based on strafe angle and gait.
	 * Higher acceleration during forward/sprint movement; lower during strafing.
	 * Influences how quickly character reaches max speed.
	 * 
	 * @param StrafeSpeedMap	Normalized strafe angle (0-1)
	 * @return				Acceleration magnitude (cm/s²)
	 */
	UFUNCTION(BlueprintPure, Category = "Movement")
	double CalculateMaxAcceleration(float StrafeSpeedMap);

	/**
	 * CalculateBrakingDeceleration
	 * 
	 * Determines deceleration rate when character releases movement input.
	 * Faster braking during forward movement; slower during strafing for planted control feel.
	 * 
	 * @param StrafeSpeedMap	Normalized strafe angle (0-1)
	 * @return				Braking deceleration magnitude (cm/s²)
	 */
	UFUNCTION(BlueprintPure, Category = "Movement")
	double CalculateBrakingDeceleration(float StrafeSpeedMap);

	/**
	 * CalculateBrakingFriction
	 * 
	 * Friction coefficient applied when character is sliding/decelerating.
	 * Higher friction value = faster velocity reduction; affects "stickiness" to ground.
	 * 
	 * @param StrafeSpeedMap	Normalized strafe angle (0-1)
	 * @return				Braking friction coefficient
	 */
	UFUNCTION(BlueprintPure, Category = "Movement")
	double CalculateBrakingFriction(float StrafeSpeedMap);

	/**
	 * CalculateGroundFriction
	 * 
	 * Friction applied while character is moving on ground during active movement input.
	 * Balances acceleration responsiveness with directional control.
	 * 
	 * @param StrafeSpeedMap	Normalized strafe angle (0-1)
	 * @return				Ground friction coefficient
	 */
	UFUNCTION(BlueprintPure, Category = "Movement")
	double CalculateGroundFriction(float StrafeSpeedMap);

	/**
	 * CalculateMaxCrouchSpeed
	 * 
	 * Maximum velocity while character is in crouched state.
	 * Typically lower than standing speeds; affected by strafe direction.
	 * 
	 * @param StrafeSpeedMap	Normalized strafe angle (0-1)
	 * @return				Maximum crouched speed (cm/s)
	 */
	UFUNCTION(BlueprintPure, Category = "Movement")
	double CalculateMaxCrouchSpeed(float StrafeSpeedMap);

	// ===== CAMERA SYSTEM =====

	/**
	 * SetupCamera
	 * 
	 * Initializes camera components and attaches them to character.
	 * Creates camera springs, offsets, and initial view targets.
	 * Must be called during character possession or PlayerController setup.
	 * 
	 * @param PlayerController	The controller possessing this character (used for view target setup)
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void SetupCamera(APlayerController* PlayerController);

	// ===== TRAVERSAL SYSTEM =====

	/**
	 * GetTraversalCheckInputs
	 * 
	 * Constructs traversal query parameters from character state and directional input.
	 * Builds FS_TraversalCheckInputs struct containing:
	 *   - Character position and orientation
	 *   - Capsule dimensions and movement traces
	 *   - Desired traversal direction (vault forward, climb up, etc.)
	 * 
	 * @param Direction		Desired traversal direction (normalized vector)
	 * @return				Populated traversal check inputs struct ready for traversal system queries
	 */
	UFUNCTION(BlueprintPure, Category = "Traversal")
	FS_TraversalCheckInputs GetTraversalCheckInputs(FVector Direction);

	/**
	 * TryTraversalAction
	 * 
	 * Main entry point for initiating traversal actions (vault, mantle, climb, rope swing, etc.).
	 * Performs collision checks, selects appropriate animation montage, and triggers traversal.
	 * Handles both client-side prediction and server replication.
	 * 
	 * @param Inputs							Traversal input parameters (position, direction, checks)
	 * @param DebugType						Debug visualization mode (None, Wireframe, Solid)
	 * @param TraversalCheckFailed			Output: true if collision/space checks failed
	 * @param MontageSelectionFailed		Output: true if no valid animation found for traversal
	 * @param TraversalCheckResult			Output: result struct with montage, warp location, etc.
	 * @param ActorLocation					Character's current position
	 * @param CapsuleRadius					Character capsule radius for collision
	 * @param CapsuleHalfHeight				Character capsule half-height for collision
	 * @param HasRoomCheck_FrontLedgeLocation	Front ledge position for space validation
	 * @param HasRoomCheck_BackLedgeLocation	Back ledge position for space validation
	 * @param TopSweepResult				Result from collision sweep used for traversal
	 * @param DrawDebugLevel				Debug visualization detail level (0-2)
	 * @param DrawDebugDuration				How long debug visualization persists (seconds)
	 * @param ValidMontages					Array of candidate montages for this traversal action
	 */
	UFUNCTION(BlueprintCallable, Category = "Traversal")
	void TryTraversalAction(
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
		TArray<UAnimMontage*> ValidMontages
	);

	/**
	 * UpdateWarpTargets
	 * 
	 * Updates motion warp targets dynamically during traversal animations.
	 * Used for rope swinging, zipline riding, and other continuous traversals.
	 * Recalculates character root position to match animated paths.
	 * 
	 * @param AnimatedDistanceFromFrontLedgeToBackLedge	Distance across traversal feature (e.g., gap width)
	 * @param AnimatedDistanceFromFrontLedgeToBackFloor	Distance to landing surface
	 */
	UFUNCTION(BlueprintCallable, Category = "Traversal")
	void UpdateWarpTargets(double AnimatedDistanceFromFrontLedgeToBackLedge, double AnimatedDistanceFromFrontLedgeToBackFloor);

	/**
	 * Traversal_ServerImplementation
	 * 
	 * Server-side implementation of traversal action execution.
	 * Receives validated traversal result from client and applies it on server for replication.
	 * Triggers montages, sets movement state, and broadcasts traversal events.
	 * 
	 * @param TraversalRep		Validated traversal result struct from client prediction
	 */
	UFUNCTION(BlueprintCallable, Category = "Traversal")
	void Traversal_ServerImplementation(FS_TraversalCheckResult TraversalRep);

	/**
	 * OnTraversalStart
	 * 
	 * Event callback triggered when traversal action begins (montage starts).
	 * Disables collision, caches velocity, optionally triggers audio/VFX.
	 * Called during montage montage begin.
	 */
	UFUNCTION(BlueprintCallable, Category = "Traversal")
	void OnTraversalStart();

	/**
	 * OnTraversalEnd
	 * 
	 * Event callback triggered when traversal action completes (montage ends).
	 * Re-enables collision, restores character control, transitions to normal locomotion.
	 * Can trigger landing effects or trigger follow-up actions.
	 */
	UFUNCTION(BlueprintCallable, Category = "Traversal")
	void OnTraversalEnd();

	/**
	 * OnRep_TraversalResult
	 * 
	 * Replication callback invoked when TraversalResult property updates on clients.
	 * Synchronizes traversal state across network for other players.
	 * Updates montage playback and motion warping on remote instances.
	 */
	UFUNCTION(BlueprintCallable, Category = "Traversal")
	void OnRep_TraversalResult();

	// ===== MOVEMENT STATE & ANIMATION =====

	/**
	 * UpdatedMovementSimulated
	 * 
	 * Called when character movement is simulated (remote/AI characters).
	 * Updates animation state based on velocity changes and ground contact.
	 * Triggers landing effects when character touches ground after airtime.
	 * 
	 * @param OldVelocity			Character velocity from previous frame
	 * @param IsMovingOnGround		True if character is currently grounded
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void UpdatedMovementSimulated(FVector OldVelocity, bool IsMovingOnGround);

	// ===== AUDIO & FEEDBACK =====

	/**
	 * PlayAudioEvent
	 * 
	 * Plays a gameplay audio event using a GameplayTag identifier.
	 * Supports volume and pitch modulation for variation (footsteps, impacts, etc.).
	 * Propagates audio on server for networked play.
	 * 
	 * @param Value				Gameplay tag identifying the audio event (e.g., "Audio.Footstep.Run")
	 * @param VolumeMultiplier	Multiplier applied to audio volume (default 1.0, range [0, 2.0])
	 * @param PitchMultiplier	Multiplier applied to audio pitch (default 1.0, range [0.5, 1.5])
	 */
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayAudioEvent(FGameplayTag Value, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f);

	// ===== ADVANCED LOCOMOTION COMPONENTS =====

	/** Grapple hook and gun component for hook-swing traversal mechanics */
	ADV_PROP_RO()
	TObjectPtr<UActorComponent> GrappleHook_Gun_Component;

	/** Interaction component for picking up items, activating switches, etc. */
	ADV_PROP_RO()
	TObjectPtr<UActorComponent> Interaction_Component;

	/** Pole climbing component for sliding and climbing vertical poles */
	ADV_PROP_RO()
	TObjectPtr<UActorComponent> PoleClimbing_Component;

	/** Push/pull component for moving physics objects and environmental mechanics */
	ADV_PROP_RO()
	TObjectPtr<UActorComponent> PushPullComponent;

	/** Narrow path (edge walk) component for balancing on thin surfaces */
	ADV_PROP_RO()
	TObjectPtr<UActorComponent> NarrowPathComponent;

	/** Sliding system component for controlled slide movements down slopes */
	ADV_PROP_RO()
	TObjectPtr<UActorComponent> SlidingComponent;

	/** Beam walk component for traversing narrow beams or rails */
	ADV_PROP_RO()
	TObjectPtr<UActorComponent> BeamWalk_Component;

	/** Zipline ascender component for climbing ziplines upward */
	ADV_PROP_RO()
	TObjectPtr<UActorComponent> ZiplineAscenderComponent;

	/** Zipline component for traversing ziplines horizontally or downward */
	ADV_PROP_RO()
	TObjectPtr<UActorComponent> ZiplineComponent;

	/** Wall running component for running along vertical surfaces */
	ADV_PROP_RO()
	TObjectPtr<UActorComponent> WallRunning_Component;

	/** Wall climbing component for scaling vertical walls hand-over-hand */
	ADV_PROP_RO()
	TObjectPtr<UActorComponent> WallClimbing_Component;

	/** Rope climbing system for swinging and climbing ropes */
	ADV_PROP_RO()
	TObjectPtr<UActorComponent> RopeClimbing_System;

	/** Swimming component for water-based locomotion */
	ADV_PROP_RO()
	TObjectPtr<UActorComponent> Swimming;

	/** Ladder system component for climbing ladders */
	ADV_PROP_RO()
	TObjectPtr<UActorComponent> Ladder_System;

	/** Physical animation component for ragdoll and dynamic animation blending */
	ADV_PROP_RO()
	TObjectPtr<UPhysicalAnimationComponent> PhysicalAnimation;

	/** General climbing component (guards, ledges, etc.) */
	ADV_PROP_RO()
	TObjectPtr<UActorComponent> Climbing_Component;

	// ===== CORE ANIMATION & CAMERA COMPONENTS =====

	/** Gameplay camera component providing camera behavior (distance, offset, FOV smoothing) */
	ADV_PROP_RO()
	TObjectPtr<UActorComponent> GameplayCamera;

	/** Pre-CMC tick component for pre-movement-component gameplay logic */
	ADV_PROP_RO()
	TObjectPtr<UActorComponent> PreCMCTick;

	/** Spring arm component providing camera distance and collision avoidance */
	ADV_PROP_RO()
	TObjectPtr<USpringArmComponent> SpringArm;

	/** Motion warping component for sync animation root motion with world positions */
	ADV_PROP_RO()
	TObjectPtr<UMotionWarpingComponent> MotionWarping;

	// ===== INPUT CONFIGURATION =====

	/** Analog stick interpretation mode (raw, exponential, squared, etc.) */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input")
	TEnumAsByte<E_AnalogStickBehavior> MovementStickMode;

	/** Active camera style (TopDown, ThirdPerson, FirstPerson) - can be toggled at runtime */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
	TEnumAsByte<E_CameraStyle> CameraStyle;

	/** Analog stick deflection threshold (0-1) above which character transitions from walk to run */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input")
	float AnalogWalk_RunThreshold;

	// ===== MOVEMENT CONFIGURATION =====

	/** Curve mapping strafe angle to speed multiplier; used for directional movement scaling */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Movement")
	TObjectPtr<UCurveFloat> StrafeSpeedMapCurve;

	/** Current locomotion gait (Walk, Run, Sprint) */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Movement")
	TEnumAsByte<E_Gait> Gait;

	/** Maximum speeds while walking: X = Forward, Y = Strafe, Z = Backward (cm/s) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	FVector WalkSpeeds;

	/** Maximum speeds while running: X = Forward, Y = Strafe, Z = Backward (cm/s) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	FVector RunSpeeds;

	/** Maximum speeds while sprinting: X = Forward, Y = Strafe, Z = Backward (cm/s) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	FVector SprintSpeeds;

	/** Maximum speeds while crouched: X = Forward, Y = Strafe, Z = Backward (cm/s) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	FVector CrouchSpeeds;

	// ===== MOVEMENT STATE & LANDING =====

	/** Flag indicating character just landed from a fall or jump this frame */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Movement")
	bool bJustLanded;

	/** Velocity vector at moment of landing; used for impact effects and landing animations */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Movement")
	FVector LandingVelocity;

	// ===== TRAVERSAL STATE (REPLICATED) =====

	/** 
	 * Result of most recent traversal check; replicated to clients using OnRep_TraversalResult.
	 * Contains montage selection, motion warp targets, and traversal metadata.
	 * 
* TODO (TODO-REPLICATION): Add to GetLifetimeReplicatedProps:
	 *   DOREPLIFETIME_WITH_PARAMS(ACBP_AdventureCharacter, TraversalResult, COND_SimulatedOnly);
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Traversal", ReplicatedUsing = "OnRep_TraversalResult")
	FS_TraversalCheckResult TraversalResult;

	/** True while traversal action (montage) is actively playing */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Traversal")
	bool bDoingTraversalAction;

	// ===== CHARACTER INPUT STATE (REPLICATED) =====

	/**
	 * Replicated input state containing movement, look, and ability activation flags.
	 * Synchronized across network to enable server-side input validation.
	 * 
* TODO (TODO-REPLICATION): Add to GetLifetimeReplicatedProps:
	 *   DOREPLIFETIME_WITH_PARAMS(ACBP_AdventureCharacter, CharacterInputState, COND_SkipOwner);
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input", Replicated)
	FCharacterInputState CharacterInputState;

	// ===== SIMULATED MOVEMENT STATE =====

	/** Cached grounded state from previous frame; used for landing detection on simulated proxies */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Movement | Simulated")
	bool bWasMovingOnGroundLastFrame_Simulated;

	/** Cached velocity from last simulated movement update; used for delta detection */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Movement | Simulated")
	FVector LastUpdateVelocity;

	// ===== ADVANCED ANIMATION OPTIONS =====

	/** When true, character root motion is driven by GAS ability attributes (damage knockback, knockdown, etc.) */
	ADV_PROP(EditDefaultsOnly, Category = "Movement")
	bool bUsingAttributeBasedRootMotion;

	/** Example of metadata usage; clamped above zero and exposed on spawn. */
	ADV_PROP(ADV_EXPOSE_ON_SPAWN(), ADV_CLAMP_MIN("0"))
	float ExampleDebugFloat;

	/** Time remaining before motion corrections are re-enabled after traversal; prevents jitter during montages */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Traversal")
	float IgnoreCorrectionsDelay;

	// ===== ADVANCED TRAVERSAL SYSTEM =====

	/** Master toggle to activate/deactivate all Advanced Traversal System mechanics */
	ADV_PROP(EditDefaultsOnly, Category = "Traversal | ATS")
	bool bATSActivate;
};
