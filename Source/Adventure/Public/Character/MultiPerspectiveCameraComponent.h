// Copyright Epic Games, Inc. All Rights Reserved.
// Multi-Perspective Camera Component - Phase 1 Camera System

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/AdventureTypes.h"
#include "MultiPerspectiveCameraComponent.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ACBP_AdventureCharacter;

/**
 * UMultiPerspectiveCameraComponent
 * 
 * Manages multi-perspective camera system for Adventure prototype (Phase 1 deliverable).
 * Provides three camera modes toggled via mouse wheel:
 *   - TopDown: Tactical overview for puzzle-solving and environmental awareness
 *   - ThirdPerson: Standard action/combat/traversal perspective
 *   - FirstPerson: Immersive investigation and detail inspection
 * 
 * Features:
 *   - Smooth camera transitions between modes (<0.5s blend requirement)
 *   - Per-mode configuration (FOV, boom length, socket offset, rotation)
 *   - Mouse wheel cycle: TopDown → ThirdPerson → FirstPerson → TopDown
 *   - Blueprint-editable camera parameters for rapid iteration
 *   - Network replicated camera state for multiplayer consistency
 * 
 * Integration:
 *   - Attaches to ACBP_AdventureCharacter's SpringArm component
 *   - Responds to IA_CameraToggle input action
 *   - Exposes current camera mode to AnimBlueprint for animation adjustments
 * 
 * Usage:
 *   1. Add component to ACBP_AdventureCharacter
 *   2. Call Initialize() with SpringArm and Camera references
 *   3. Bind CycleCamera() to mouse wheel scroll input
 *   4. Call SetCameraMode() or CycleCamera() to switch perspectives
 * 
 * @see E_CameraStyle enum for available modes
 * @see ACBP_AdventureCharacter for owner character class
 */
UCLASS(ClassGroup=(Camera), meta=(BlueprintSpawnableComponent))
class ADVENTURE_API UMultiPerspectiveCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMultiPerspectiveCameraComponent();

	// ===== INITIALIZATION =====

	/**
	 * Initialize camera component with SpringArm and Camera references
	 * Must be called after character construction
	 * 
	 * @param InSpringArm		Character's SpringArm component
	 * @param InCamera			Character's Camera component
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void Initialize(USpringArmComponent* InSpringArm, UCameraComponent* InCamera);

	// ===== CAMERA MODE CONTROL =====

	/**
	 * Cycle to next camera mode in sequence (TopDown → ThirdPerson → FirstPerson → TopDown)
	 * Called when player scrolls mouse wheel or presses camera toggle button
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void CycleCamera();

	/**
	 * Set camera to specific mode with smooth transition
	 * 
	 * @param NewMode	Target camera style to switch to
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void SetCameraMode(E_CameraStyle NewMode);

	/**
	 * Get current active camera mode
	 * 
	 * @return Current camera style (TopDown, ThirdPerson, or FirstPerson)
	 */
	UFUNCTION(BlueprintPure, Category = "Camera")
	E_CameraStyle GetCurrentCameraMode() const { return CurrentCameraMode; }

	/**
	 * Check if camera is currently transitioning between modes
	 * 
	 * @return True if blend is in progress, false if settled
	 */
	UFUNCTION(BlueprintPure, Category = "Camera")
	bool IsTransitioning() const { return bIsTransitioning; }

protected:
	// ===== TICK & UPDATE =====

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ===== TRANSITION LOGIC =====

	/**
	 * Update smooth camera transition blend
	 * Interpolates SpringArm length, socket offset, rotation, and Camera FOV
	 * 
	 * @param DeltaTime		Time since last frame (seconds)
	 */
	void UpdateCameraTransition(float DeltaTime);

	/**
	 * Apply camera parameters for specified mode instantly (no blend)
	 * 
	 * @param Mode		Camera mode to apply
	 */
	void ApplyCameraSettings(E_CameraStyle Mode);

	/**
	 * Get next camera mode in cycle sequence
	 * 
	 * @param CurrentMode	Current camera style
	 * @return Next camera style in cycle
	 */
	static E_CameraStyle GetNextCameraMode(E_CameraStyle CurrentMode);

public:
	// ===== CAMERA CONFIGURATION (TopDown Mode) =====

	/** SpringArm boom length for TopDown camera (cm) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|TopDown")
	float TopDown_BoomLength = 1200.0f;

	/** SpringArm socket offset for TopDown camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|TopDown")
	FVector TopDown_SocketOffset = FVector(0.0f, 0.0f, 100.0f);

	/** SpringArm rotation for TopDown camera (pitch typically -60 to -80) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|TopDown")
	FRotator TopDown_Rotation = FRotator(-70.0f, 0.0f, 0.0f);

	/** Camera field of view for TopDown mode (degrees) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|TopDown")
	float TopDown_FOV = 90.0f;

	/** Whether SpringArm should inherit pitch rotation from pawn in TopDown mode */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|TopDown")
	bool TopDown_InheritPitch = false;

	/** Whether SpringArm should inherit yaw rotation from pawn in TopDown mode */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|TopDown")
	bool TopDown_InheritYaw = false;

	// ===== CAMERA CONFIGURATION (ThirdPerson Mode) =====

	/** SpringArm boom length for ThirdPerson camera (cm) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|ThirdPerson")
	float ThirdPerson_BoomLength = 400.0f;

	/** SpringArm socket offset for ThirdPerson camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|ThirdPerson")
	FVector ThirdPerson_SocketOffset = FVector(0.0f, 50.0f, 70.0f);

	/** SpringArm rotation for ThirdPerson camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|ThirdPerson")
	FRotator ThirdPerson_Rotation = FRotator(-15.0f, 0.0f, 0.0f);

	/** Camera field of view for ThirdPerson mode (degrees) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|ThirdPerson")
	float ThirdPerson_FOV = 90.0f;

	/** Whether SpringArm should inherit pitch rotation from pawn in ThirdPerson mode */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|ThirdPerson")
	bool ThirdPerson_InheritPitch = true;

	/** Whether SpringArm should inherit yaw rotation from pawn in ThirdPerson mode */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|ThirdPerson")
	bool ThirdPerson_InheritYaw = true;

	// ===== CAMERA CONFIGURATION (FirstPerson Mode) =====

	/** SpringArm boom length for FirstPerson camera (cm) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|FirstPerson")
	float FirstPerson_BoomLength = 0.0f;

	/** SpringArm socket offset for FirstPerson camera (typically at eye level) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|FirstPerson")
	FVector FirstPerson_SocketOffset = FVector(0.0f, 0.0f, 80.0f);

	/** SpringArm rotation for FirstPerson camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|FirstPerson")
	FRotator FirstPerson_Rotation = FRotator(0.0f, 0.0f, 0.0f);

	/** Camera field of view for FirstPerson mode (degrees) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|FirstPerson")
	float FirstPerson_FOV = 90.0f;

	/** Whether SpringArm should inherit pitch rotation from pawn in FirstPerson mode */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|FirstPerson")
	bool FirstPerson_InheritPitch = true;

	/** Whether SpringArm should inherit yaw rotation from pawn in FirstPerson mode */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|FirstPerson")
	bool FirstPerson_InheritYaw = true;

	// ===== TRANSITION SETTINGS =====

	/** Transition blend duration when switching camera modes (seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Transition")
	float TransitionDuration = 0.4f; // < 0.5s requirement per acceptance criteria

	/** Curve for camera transition interpolation (default: smooth ease-in-out) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Transition")
	float TransitionCurveExponent = 2.0f; // 2.0 = smooth quadratic interpolation

private:
	// ===== COMPONENT REFERENCES =====

	/** Reference to character's SpringArm component */
	UPROPERTY()
	USpringArmComponent* SpringArm = nullptr;

	/** Reference to character's Camera component */
	UPROPERTY()
	UCameraComponent* Camera = nullptr;

	/** Reference to owning character */
	UPROPERTY()
	ACBP_AdventureCharacter* OwnerCharacter = nullptr;

	// ===== TRANSITION STATE =====

	/** Current active camera mode */
	E_CameraStyle CurrentCameraMode = E_CameraStyle::ThirdPerson;

	/** Target camera mode during transition */
	E_CameraStyle TargetCameraMode = E_CameraStyle::ThirdPerson;

	/** Whether transition is currently in progress */
	bool bIsTransitioning = false;

	/** Elapsed time during current transition (seconds) */
	float TransitionElapsedTime = 0.0f;

	// ===== TRANSITION INTERPOLATION CACHE =====

	/** Starting boom length for current transition */
	float Transition_StartBoomLength = 0.0f;

	/** Starting socket offset for current transition */
	FVector Transition_StartSocketOffset = FVector::ZeroVector;

	/** Starting rotation for current transition */
	FRotator Transition_StartRotation = FRotator::ZeroRotator;

	/** Starting FOV for current transition */
	float Transition_StartFOV = 90.0f;
};
