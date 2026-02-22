// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Core Game Mode Implementation

#include "Core/GM_AdventureMode.h"
#include "Character/CBP_AdventureCharacter.h"
#include "Character/CBP_AdventureCharacter_Mover.h"
#include "Core/AdventurePlayerState.h"
#include "Core/PC_AdventureController.h"
#include "GameFramework/PlayerController.h"
#include "Curves/CurveFloat.h"
#include "GameplayTagsManager.h"

AGM_AdventureMode::AGM_AdventureMode()
{
	// Set default player controller and character class
	// NOTE: Using Mover2 variant (ACBP_AdventureCharacter_Mover) as default pawn
	// for advanced movement system. Can toggle between CMC and Mover2 via blueprint override.
	PlayerControllerClass = APC_AdventureController::StaticClass();
	DefaultPawnClass = ACBP_AdventureCharacter_Mover::StaticClass();
	PlayerStateClass = AAdventurePlayerState::StaticClass();

	// Initialize game configuration
	DamageMultiplier = 1.0f;
	EnemyHealthMultiplier = 1.0f;
	AbilityCostMultiplier = 1.0f;
	bEnableFriendlyFire = false;
	bEnableRespawn = true;
	RespawnDelay = 3.0f;
	bHUDMinimalMode = false;

	// Initialize game state
	CurrentDifficulty = 1; // Normal
	bGamePaused = false;
	bGameOver = false;
	bHUDVisible = true;

	// Create root scene component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	DefaultSceneRoot = RootComponent;
}

void AGM_AdventureMode::BeginPlay()
{
	Super::BeginPlay();

	// TODO: Initialize game parameters
	// TODO: Spawn initial enemies
	// TODO: Setup level-specific configuration
	// TODO: Trigger opening cinematics or gameplay start
}

void AGM_AdventureMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// TODO: Initialize player-specific settings
	// TODO: Bind player to HUD
	// TODO: Setup camera preferences
}

void AGM_AdventureMode::ReturnToMainMenu()
{
	// TODO: Transition back to main menu
	// TODO: Cleanup level resources
	// TODO: Save any persistent state if needed
}

void AGM_AdventureMode::RestartGame()
{
	// TODO: Reload current level
	// TODO: Reset player state
	// TODO: Respawn enemies
}

void AGM_AdventureMode::PauseGame(bool bPause)
{
	bGamePaused = bPause;
	GetWorldSettings()->SetTimeDilation(bPause ? 0.0f : 1.0f);

	// TODO: Update HUD pause state
	// TODO: Disable/enable AI
}

bool AGM_AdventureMode::GetIsGameOver() const
{
	return bGameOver;
}

void AGM_AdventureMode::SetGameOver(bool bGameOverState)
{
	bGameOver = bGameOverState;

	if (bGameOver)
	{
		// TODO: Disable player input
		// TODO: Trigger defeat VFX/audio
		// TODO: Show death/retry UI
	}
}

float AGM_AdventureMode::GetDifficultyMultiplier() const
{
	// Convert difficulty index to multiplier
	switch (CurrentDifficulty)
	{
		case 0: return 0.5f;   // Easy
		case 1: return 1.0f;   // Normal
		case 2: return 2.0f;   // Hard
		default: return 1.0f;
	}
}

void AGM_AdventureMode::SetDifficulty(int32 NewDifficulty)
{
	CurrentDifficulty = FMath::Max(0, FMath::Min(2, NewDifficulty));
	DamageMultiplier = GetDifficultyMultiplier();

	// TODO: Update enemy stats
	// TODO: Adjust ability costs
	// TODO: Update HUD difficulty indicator
}

void AGM_AdventureMode::ToggleHUDVisibility()
{
	bHUDVisible = !bHUDVisible;

	// TODO: Show/hide HUD widget
}

bool AGM_AdventureMode::GetHUDVisible() const
{
	return bHUDVisible;
}

APC_AdventureController* AGM_AdventureMode::GetAdventurePlayerController() const
{
	// TODO: Properly cast GetGameMode()->GetPrimaryPlayerController() to APC_AdventureController
	// Simplified: return nullptr as placeholder
	return nullptr;
}

ACBP_AdventureCharacter* AGM_AdventureMode::GetAdventureCharacter() const
{
	// TODO: Return player pawn as ACBP_AdventureCharacter
	APC_AdventureController* PC = GetAdventurePlayerController();
	if (PC)
	{
		return Cast<ACBP_AdventureCharacter>(PC->GetPawn());
	}
	return nullptr;
}

void AGM_AdventureMode::PlayGameAudioEvent(FGameplayTag AudioTag, float VolumeMultiplier)
{
	// TODO: Play global audio event
}
