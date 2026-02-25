// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Core Player Controller Implementation

#include "Core/PC_AdventureController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Character/CBP_AdventureCharacter.h"
#include "Character/CBP_AdventureCharacter_Mover.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// forward declaration to avoid include
class ULocalPlayer;
#include "GAS/AdventureGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"

APC_AdventureController::APC_AdventureController()
{
	// Initialize input settings
	LookSensitivity = 1.0f;
	bInvertYAxis = false;
	bUseGamepadDeadzone = true;
	GamepadDeadzoneThreshold = 0.1f;

	// Initialize camera state
	CurrentCameraStyleIndex = 1; // ThirdPerson default

	// Initialize pause state
	bGamePaused = false;
}

void APC_AdventureController::BeginPlay()
{
	Super::BeginPlay();

	// HUD disabled in rollback baseline
}

void APC_AdventureController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Cache adventure character reference
	CachedAdventureCharacter = Cast<ACBP_AdventureCharacter>(InPawn);
	CachedAdventureMover = Cast<ACBP_AdventureCharacter_Mover>(InPawn);
	if (CachedAdventureCharacter)
	{
		if (IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(InPawn))
		{
			CachedAbilitySystemComponent = AbilityInterface->GetAbilitySystemComponent();
		}

		// Setup Enhanced Input System
		SetupEnhancedInput();

		// Setup character components (camera, input, etc.)
		if (CachedAdventureCharacter)
		{
			CachedAdventureCharacter->SetupCamera(this);
			CachedAdventureCharacter->SetupInput();
		}
	}
	else if (CachedAdventureMover)
	{
		if (IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(InPawn))
		{
			CachedAbilitySystemComponent = AbilityInterface->GetAbilitySystemComponent();
		}

		SetupEnhancedInput();
	}
}

void APC_AdventureController::OnUnPossess()
{
	// Cleanup Enhanced Input
	TeardownEnhancedInput();

	// Clear cached references
	CachedAdventureCharacter = nullptr;
	CachedAdventureMover = nullptr;
	CachedAbilitySystemComponent = nullptr;

	Super::OnUnPossess();
}

void APC_AdventureController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// TODO: Bind Enhanced Input actions in derived Blueprint class
	// This is handled in SetupEnhancedInput for EIS integration
}

void APC_AdventureController::SetupEnhancedInput()
{
	// Enhanced Input disabled in rollback baseline; nothing to do
}

void APC_AdventureController::TeardownEnhancedInput()
{
	// No-op for baseline
}

void APC_AdventureController::OnMoveInput(const FInputActionValue& Value)
{
	// TODO: Pass movement input to possessed character
	if (CachedAdventureCharacter)
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();
		// TODO: Call character movement update
	}
	else if (CachedAdventureMover)
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();
		CachedAdventureMover->SetMoveInputVector(MovementVector);
	}
}

void APC_AdventureController::OnLookInput(const FInputActionValue& Value)
{
	// TODO: Update camera rotation based on look input
	const FVector2D LookVector = Value.Get<FVector2D>();
	// TODO: Apply sensitivity and inversion settings

	if (CachedAdventureMover)
	{
		CachedAdventureMover->SetLookInputVector(LookVector);
	}
}

void APC_AdventureController::OnSprintInput(const FInputActionValue& Value)
{
	TryActivateAbilityByTag(AdventureGameplayTags::Ability_Sprint);
}

void APC_AdventureController::OnDodgeInput(const FInputActionValue& Value)
{
	TryActivateAbilityByTag(AdventureGameplayTags::Ability_Dodge);
}

void APC_AdventureController::OnAbilityInput_Light(const FInputActionValue& Value)
{
	// TODO: Activate light attack ability (GA_LightAttack)
	if (CachedAbilitySystemComponent)
	{
		// TODO: Activate ability by tag
	}
}

void APC_AdventureController::OnAbilityInput_Heavy(const FInputActionValue& Value)
{
	TryActivateAbilityByTag(AdventureGameplayTags::Ability_Traversal);
}

void APC_AdventureController::OnInteractInput(const FInputActionValue& Value)
{
	TryActivateAbilityByTag(AdventureGameplayTags::Ability_Interact);
}

void APC_AdventureController::TryActivateAbilityByTag(FGameplayTag AbilityTag)
{
	if (CachedAbilitySystemComponent && AbilityTag.IsValid())
	{
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(AbilityTag);
		CachedAbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
	}
}

void APC_AdventureController::OnCameraToggleInput(const FInputActionValue& Value)
{
	CycleCamera();
}

void APC_AdventureController::OnPauseInput(const FInputActionValue& Value)
{
	SetGamePaused(!bGamePaused);
}



ACBP_AdventureCharacter* APC_AdventureController::GetAdventureCharacter() const
{
	return CachedAdventureCharacter;
}

UAbilitySystemComponent* APC_AdventureController::GetAdventureCharacterAbilitySystem() const
{
	return CachedAbilitySystemComponent;
}

bool APC_AdventureController::IsGamePaused() const
{
	return bGamePaused;
}

void APC_AdventureController::SetGamePaused(bool bPause)
{
	bGamePaused = bPause;

	// TODO: Update game world time dilation
	// TODO: Update HUD pause menu visibility
	// TODO: Disable/enable AI
}

void APC_AdventureController::CycleCamera()
{
	if (!CachedAdventureCharacter)
	{
		return;
	}

	// Cycle to next camera mode (TopDown → ThirdPerson → FirstPerson → TopDown)
	// Camera modes: 0 = TopDown, 1 = ThirdPerson, 2 = FirstPerson
	CurrentCameraStyleIndex = (CurrentCameraStyleIndex + 1) % 3;

	// Update character's camera style
	E_CameraStyle NewCameraStyle = static_cast<E_CameraStyle>(CurrentCameraStyleIndex);
	CachedAdventureCharacter->CameraStyle = NewCameraStyle;

	// Log the camera change
	const FString CameraModeName = (NewCameraStyle == E_CameraStyle::TopDown) ? TEXT("Top-Down") :
		(NewCameraStyle == E_CameraStyle::ThirdPerson) ? TEXT("Third-Person") :
		TEXT("First-Person");
	
	UE_LOG(LogTemp, Log, TEXT("APC_AdventureController::CycleCamera - Switched to %s camera"), *CameraModeName);

	// TODO: Add smooth camera transition animation
	// TODO: Update HUD to display current camera mode
}

int32 APC_AdventureController::GetCurrentCameraStyle() const
{
	return CurrentCameraStyleIndex;
}
