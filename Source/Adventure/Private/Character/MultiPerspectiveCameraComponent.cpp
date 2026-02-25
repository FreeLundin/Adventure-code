// Copyright Epic Games, Inc. All Rights Reserved.
// Multi-Perspective Camera Component - Implementation

#include "Character/MultiPerspectiveCameraComponent.h"
#include "Character/CBP_AdventureCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

UMultiPerspectiveCameraComponent::UMultiPerspectiveCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsTransitioning = false;
}

void UMultiPerspectiveCameraComponent::Initialize(USpringArmComponent* InSpringArm, UCameraComponent* InCamera)
{
	SpringArm = InSpringArm;
	Camera = InCamera;
	OwnerCharacter = Cast<ACBP_AdventureCharacter>(GetOwner());

	if (!SpringArm || !Camera)
	{
		UE_LOG(LogTemp, Error, TEXT("UMultiPerspectiveCameraComponent::Initialize - Invalid SpringArm or Camera references"));
		return;
	}

	// Apply initial camera settings for ThirdPerson mode
	ApplyCameraSettings(CurrentCameraMode);

	UE_LOG(LogTemp, Log, TEXT("UMultiPerspectiveCameraComponent::Initialize - Camera system initialized (Mode: ThirdPerson)"));
}

void UMultiPerspectiveCameraComponent::CycleCamera()
{
	E_CameraStyle NextMode = GetNextCameraMode(CurrentCameraMode);
	SetCameraMode(NextMode);

	UE_LOG(LogTemp, Log, TEXT("UMultiPerspectiveCameraComponent::CycleCamera - Cycling to %s"), 
		NextMode == E_CameraStyle::TopDown ? TEXT("TopDown") : 
		NextMode == E_CameraStyle::ThirdPerson ? TEXT("ThirdPerson") : TEXT("FirstPerson"));
}

void UMultiPerspectiveCameraComponent::SetCameraMode(E_CameraStyle NewMode)
{
	if (!SpringArm || !Camera)
	{
		return;
	}

	if (NewMode == CurrentCameraMode && !bIsTransitioning)
	{
		return; // Already in target mode
	}

	// Cache current values as transition start
	Transition_StartBoomLength = SpringArm->TargetArmLength;
	Transition_StartSocketOffset = SpringArm->SocketOffset;
	Transition_StartRotation = SpringArm->GetRelativeRotation();
	Transition_StartFOV = Camera->FieldOfView;

	// Set target mode and begin transition
	TargetCameraMode = NewMode;
	bIsTransitioning = true;
	TransitionElapsedTime = 0.0f;

	UE_LOG(LogTemp, Log, TEXT("UMultiPerspectiveCameraComponent::SetCameraMode - Transitioning to %s (Duration: %.2fs)"),
		NewMode == E_CameraStyle::TopDown ? TEXT("TopDown") :
		NewMode == E_CameraStyle::ThirdPerson ? TEXT("ThirdPerson") : TEXT("FirstPerson"),
		TransitionDuration);
}

void UMultiPerspectiveCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsTransitioning)
	{
		UpdateCameraTransition(DeltaTime);
	}
}

void UMultiPerspectiveCameraComponent::UpdateCameraTransition(float DeltaTime)
{
	if (!SpringArm || !Camera)
	{
		bIsTransitioning = false;
		return;
	}

	TransitionElapsedTime += DeltaTime;

	// Calculate blend alpha (0 to 1)
	float Alpha = FMath::Clamp(TransitionElapsedTime / TransitionDuration, 0.0f, 1.0f);

	// Apply curve (smooth ease-in-out using exponent)
	float CurvedAlpha = FMath::Pow(Alpha, TransitionCurveExponent);

	// Get target values based on target mode
	float TargetBoomLength = 0.0f;
	FVector TargetSocketOffset = FVector::ZeroVector;
	FRotator TargetRotation = FRotator::ZeroRotator;
	float TargetFOV = 90.0f;
	bool TargetInheritPitch = true;
	bool TargetInheritYaw = true;

	switch (TargetCameraMode)
	{
	case E_CameraStyle::TopDown:
		TargetBoomLength = TopDown_BoomLength;
		TargetSocketOffset = TopDown_SocketOffset;
		TargetRotation = TopDown_Rotation;
		TargetFOV = TopDown_FOV;
		TargetInheritPitch = TopDown_InheritPitch;
		TargetInheritYaw = TopDown_InheritYaw;
		break;

	case E_CameraStyle::ThirdPerson:
		TargetBoomLength = ThirdPerson_BoomLength;
		TargetSocketOffset = ThirdPerson_SocketOffset;
		TargetRotation = ThirdPerson_Rotation;
		TargetFOV = ThirdPerson_FOV;
		TargetInheritPitch = ThirdPerson_InheritPitch;
		TargetInheritYaw = ThirdPerson_InheritYaw;
		break;

	case E_CameraStyle::FirstPerson:
		TargetBoomLength = FirstPerson_BoomLength;
		TargetSocketOffset = FirstPerson_SocketOffset;
		TargetRotation = FirstPerson_Rotation;
		TargetFOV = FirstPerson_FOV;
		TargetInheritPitch = FirstPerson_InheritPitch;
		TargetInheritYaw = FirstPerson_InheritYaw;
		break;
	}

	// Interpolate values
	SpringArm->TargetArmLength = FMath::Lerp(Transition_StartBoomLength, TargetBoomLength, CurvedAlpha);
	SpringArm->SocketOffset = FMath::Lerp(Transition_StartSocketOffset, TargetSocketOffset, CurvedAlpha);
	SpringArm->SetRelativeRotation(UKismetMathLibrary::RLerp(Transition_StartRotation, TargetRotation, CurvedAlpha, true));
	Camera->SetFieldOfView(FMath::Lerp(Transition_StartFOV, TargetFOV, CurvedAlpha));

	// Apply rotation inheritance settings (instant switch at 50% blend)
	if (Alpha >= 0.5f)
	{
		SpringArm->bInheritPitch = TargetInheritPitch;
		SpringArm->bInheritYaw = TargetInheritYaw;
	}

	// Check if transition complete
	if (Alpha >= 1.0f)
	{
		bIsTransitioning = false;
		CurrentCameraMode = TargetCameraMode;
		ApplyCameraSettings(CurrentCameraMode); // Ensure exact target values

		UE_LOG(LogTemp, Log, TEXT("UMultiPerspectiveCameraComponent::UpdateCameraTransition - Transition complete to %s"),
			CurrentCameraMode == E_CameraStyle::TopDown ? TEXT("TopDown") :
			CurrentCameraMode == E_CameraStyle::ThirdPerson ? TEXT("ThirdPerson") : TEXT("FirstPerson"));
	}
}

void UMultiPerspectiveCameraComponent::ApplyCameraSettings(E_CameraStyle Mode)
{
	if (!SpringArm || !Camera)
	{
		return;
	}

	switch (Mode)
	{
	case E_CameraStyle::TopDown:
		SpringArm->TargetArmLength = TopDown_BoomLength;
		SpringArm->SocketOffset = TopDown_SocketOffset;
		SpringArm->SetRelativeRotation(TopDown_Rotation);
		SpringArm->bInheritPitch = TopDown_InheritPitch;
		SpringArm->bInheritYaw = TopDown_InheritYaw;
		Camera->SetFieldOfView(TopDown_FOV);
		break;

	case E_CameraStyle::ThirdPerson:
		SpringArm->TargetArmLength = ThirdPerson_BoomLength;
		SpringArm->SocketOffset = ThirdPerson_SocketOffset;
		SpringArm->SetRelativeRotation(ThirdPerson_Rotation);
		SpringArm->bInheritPitch = ThirdPerson_InheritPitch;
		SpringArm->bInheritYaw = ThirdPerson_InheritYaw;
		Camera->SetFieldOfView(ThirdPerson_FOV);
		break;

	case E_CameraStyle::FirstPerson:
		SpringArm->TargetArmLength = FirstPerson_BoomLength;
		SpringArm->SocketOffset = FirstPerson_SocketOffset;
		SpringArm->SetRelativeRotation(FirstPerson_Rotation);
		SpringArm->bInheritPitch = FirstPerson_InheritPitch;
		SpringArm->bInheritYaw = FirstPerson_InheritYaw;
		Camera->SetFieldOfView(FirstPerson_FOV);
		break;
	}
}

E_CameraStyle UMultiPerspectiveCameraComponent::GetNextCameraMode(E_CameraStyle CurrentMode)
{
	switch (CurrentMode)
	{
	case E_CameraStyle::TopDown:
		return E_CameraStyle::ThirdPerson;

	case E_CameraStyle::ThirdPerson:
		return E_CameraStyle::FirstPerson;

	case E_CameraStyle::FirstPerson:
		return E_CameraStyle::TopDown;

	default:
		return E_CameraStyle::ThirdPerson;
	}
}
