// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Mover-based Character Variant

#include "Character/CBP_AdventureCharacter_Mover.h"
#include "Character/CBP_AdventureCharacter.h" // traversal structs
#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Core/AdventurePlayerState.h"
#include "DefaultMovementSet/CharacterMoverComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/AdventureDefaultAttributesEffect.h"
#include "GAS/AdventureGameplayAbility.h"
#include "InputAction.h"
#include "InputActionValue.h"

ACBP_AdventureCharacter_Mover::ACBP_AdventureCharacter_Mover()
{
	PrimaryActorTick.bCanEverTick = true;

	SetReplicatingMovement(false);

	AbilitySystemComponent = nullptr;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	CharacterMoverComponent = CreateDefaultSubobject<UCharacterMoverComponent>(TEXT("CharacterMoverComponent"));
	CharacterMoverComponent->SetUpdatedComponent(CapsuleComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	DefaultAbilities = {
		UGA_AdventureSprint::StaticClass(),
		UGA_AdventureDodge::StaticClass(),
		UGA_AdventureTraversal::StaticClass(),
		UGA_AdventureInteract::StaticClass()
	};

	DefaultAttributesEffect = UGE_AdventureDefaultAttributes::StaticClass();
}

void ACBP_AdventureCharacter_Mover::BeginPlay()
{
	Super::BeginPlay();
}

void ACBP_AdventureCharacter_Mover::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!CharacterMoverComponent)
	{
		CharacterMoverComponent = FindComponentByClass<UCharacterMoverComponent>();
	}
}

void ACBP_AdventureCharacter_Mover::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitializeAbilitySystem();
}

void ACBP_AdventureCharacter_Mover::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitializeAbilitySystem();
}

UAbilitySystemComponent* ACBP_AdventureCharacter_Mover::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACBP_AdventureCharacter_Mover::InitializeAbilitySystem()
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

void ACBP_AdventureCharacter_Mover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		const float LookRate = 100.0f * LookSensitivity;
		const float PitchScale = bInvertLookY ? 1.0f : -1.0f;

		PC->AddYawInput(CachedLookInput.X * LookRate * DeltaTime);
		PC->AddPitchInput(CachedLookInput.Y * LookRate * PitchScale * DeltaTime);
	}

	CachedLookInput = FVector2D::ZeroVector;
}

void ACBP_AdventureCharacter_Mover::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveInputAction)
		{
			Input->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ACBP_AdventureCharacter_Mover::OnMoveTriggered);
			Input->BindAction(MoveInputAction, ETriggerEvent::Completed, this, &ACBP_AdventureCharacter_Mover::OnMoveCompleted);
		}

		if (LookInputAction)
		{
			Input->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ACBP_AdventureCharacter_Mover::OnLookTriggered);
			Input->BindAction(LookInputAction, ETriggerEvent::Completed, this, &ACBP_AdventureCharacter_Mover::OnLookCompleted);
		}

		if (JumpInputAction)
		{
			Input->BindAction(JumpInputAction, ETriggerEvent::Started, this, &ACBP_AdventureCharacter_Mover::OnJumpStarted);
			Input->BindAction(JumpInputAction, ETriggerEvent::Completed, this, &ACBP_AdventureCharacter_Mover::OnJumpReleased);
		}

		if (SprintInputAction)
		{
			Input->BindAction(SprintInputAction, ETriggerEvent::Started, this, &ACBP_AdventureCharacter_Mover::OnSprintStarted);
			Input->BindAction(SprintInputAction, ETriggerEvent::Completed, this, &ACBP_AdventureCharacter_Mover::OnSprintReleased);
		}

		if (CrouchInputAction)
		{
			Input->BindAction(CrouchInputAction, ETriggerEvent::Started, this, &ACBP_AdventureCharacter_Mover::OnCrouchStarted);
			Input->BindAction(CrouchInputAction, ETriggerEvent::Completed, this, &ACBP_AdventureCharacter_Mover::OnCrouchReleased);
		}
	}
}

void ACBP_AdventureCharacter_Mover::ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult)
{
	OnProduceInput(static_cast<float>(SimTimeMs), InputCmdResult);

	FCharacterDefaultInputs& CharacterInputs = InputCmdResult.InputCollection.FindOrAddMutableDataByType<FCharacterDefaultInputs>();

	CharacterInputs.SetMoveInput(EMoveInputType::DirectionalIntent, CachedMoveInputIntent);
	CharacterInputs.OrientationIntent = CachedMoveInputIntent.IsNearlyZero() ? FVector::ZeroVector : CachedMoveInputIntent.GetSafeNormal();
	CharacterInputs.ControlRotation = GetControlRotation();
	CharacterInputs.bIsJumpJustPressed = bJumpJustPressed;
	CharacterInputs.bIsJumpPressed = bJumpHeld;

	bJumpJustPressed = false;

	InputCmdResult = OnProduceInputInBlueprint(static_cast<float>(SimTimeMs), InputCmdResult);
}

void ACBP_AdventureCharacter_Mover::OnProduceInput(float DeltaMs, FMoverInputCmdContext& InputCmdResult)
{
	// Optional override for native extension of input production
}

void ACBP_AdventureCharacter_Mover::SetMoveInputVector(const FVector2D& MoveInput)
{
	CachedMoveInputIntent = FVector(MoveInput.X, MoveInput.Y, 0.0f);
}

void ACBP_AdventureCharacter_Mover::SetLookInputVector(const FVector2D& LookInput)
{
	CachedLookInput = LookInput;
}

void ACBP_AdventureCharacter_Mover::SetJumpPressed(bool bPressed)
{
	bJumpJustPressed = bPressed && !bJumpHeld;
	bJumpHeld = bPressed;
}

void ACBP_AdventureCharacter_Mover::SetSprintPressed(bool bPressed)
{
	bSprintHeld = bPressed;
}

void ACBP_AdventureCharacter_Mover::SetCrouchPressed(bool bPressed)
{
	bCrouchHeld = bPressed;
}

void ACBP_AdventureCharacter_Mover::RequestTraversalVault()
{
	OnTraversalRequestVault();
}

void ACBP_AdventureCharacter_Mover::OnTraversalRequestVault_Implementation()
{
	// default native behaviour: forward to base character traversal query using forward direction
	UE_LOG(LogTemp, Log, TEXT("[Traversal] Mover vault request"));
	FVector Dir = GetActorForwardVector();
	FS_TraversalCheckInputs Inputs = GetTraversalCheckInputs(Dir);
	bool bTraversalCheckFailed = true;
	bool bMontageSelectionFailed = true;
	FS_TraversalCheckResult Result;
	TryTraversalAction(Inputs, false, bTraversalCheckFailed, bMontageSelectionFailed, Result,
		GetActorLocation(),
		GetCapsuleComponent()->GetScaledCapsuleRadius(),
		GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
		FVector::ZeroVector, FVector::ZeroVector,
		FHitResult(), 0, 0.0, TArray<UAnimMontage*>());
}

void ACBP_AdventureCharacter_Mover::OnTraversalRequestMantle_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("[Traversal] Mover mantle request"));
	// reuse same logic for now
	OnTraversalRequestVault_Implementation();
}

void ACBP_AdventureCharacter_Mover::OnTraversalRequestClimb_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("[Traversal] Mover climb request"));
	OnTraversalRequestVault_Implementation();
}
void ACBP_AdventureCharacter_Mover::RequestTraversalMantle()
{
	OnTraversalRequestMantle();
}

void ACBP_AdventureCharacter_Mover::RequestTraversalClimb()
{
	OnTraversalRequestClimb();
}

void ACBP_AdventureCharacter_Mover::OnMoveTriggered(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();
	SetMoveInputVector(Input);
}

void ACBP_AdventureCharacter_Mover::OnMoveCompleted(const FInputActionValue& Value)
{
	SetMoveInputVector(FVector2D::ZeroVector);
}

void ACBP_AdventureCharacter_Mover::OnLookTriggered(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();
	SetLookInputVector(Input);
}

void ACBP_AdventureCharacter_Mover::OnLookCompleted(const FInputActionValue& Value)
{
	SetLookInputVector(FVector2D::ZeroVector);
}

void ACBP_AdventureCharacter_Mover::OnJumpStarted(const FInputActionValue& Value)
{
	SetJumpPressed(true);
}

void ACBP_AdventureCharacter_Mover::OnJumpReleased(const FInputActionValue& Value)
{
	SetJumpPressed(false);
}

void ACBP_AdventureCharacter_Mover::OnSprintStarted(const FInputActionValue& Value)
{
	SetSprintPressed(true);
}

void ACBP_AdventureCharacter_Mover::OnSprintReleased(const FInputActionValue& Value)
{
	SetSprintPressed(false);
}

void ACBP_AdventureCharacter_Mover::OnCrouchStarted(const FInputActionValue& Value)
{
	SetCrouchPressed(true);
}

void ACBP_AdventureCharacter_Mover::OnCrouchReleased(const FInputActionValue& Value)
{
	SetCrouchPressed(false);
}
