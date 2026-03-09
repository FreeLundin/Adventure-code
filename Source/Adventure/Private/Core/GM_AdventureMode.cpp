// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Core Game Mode Implementation

#include "Core/GM_AdventureMode.h"
#include "Character/CBP_AdventureCharacter.h"
#include "Character/CBP_AdventureCharacter_Mover.h"
#include "Core/AdventurePlayerState.h"
#include "Core/PC_AdventureController.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/WorldSettings.h" // needed for GetWorldSettings()
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
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

	// initialize parameters from config or data table if available
	UE_LOG(LogTemp, Log, TEXT("AGM_AdventureMode::BeginPlay - initializing game parameters"));

	// spawn enemies via spawner actors placed in level (handled by blueprints)
	UE_LOG(LogTemp, Log, TEXT("AGM_AdventureMode::BeginPlay - ready to spawn initial enemies"));

	// apply any level-specific overrides (e.g. difficulty, environment)
	UE_LOG(LogTemp, Log, TEXT("AGM_AdventureMode::BeginPlay - applying level configuration"));

	// opening cinematic could be triggered by the level blueprint, notify camera manager
	UE_LOG(LogTemp, Log, TEXT("AGM_AdventureMode::BeginPlay - gameplay start/cinematic trigger point"));
}

void AGM_AdventureMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Log, TEXT("AGM_AdventureMode::PostLogin - setting up new player controller"));

	if (APC_AdventureController *AdvCtrl = Cast<APC_AdventureController>(NewPlayer))
	{
		// HUD binding is handled by controller's BeginPlay implementation
		UE_LOG(LogTemp, Verbose, TEXT("Player controller is AdventureController, HUD will spawn automatically"));

		// camera prefs may be stored on player state or controller settings
		// leave as a stub for now
	}
}

void AGM_AdventureMode::ReturnToMainMenu()
{
	UE_LOG(LogTemp, Log, TEXT("AGM_AdventureMode::ReturnToMainMenu - returning to menu (not implemented)"));
	// placeholder: implement flow in blueprint or game instance
}

void AGM_AdventureMode::RestartGame()
{
	UE_LOG(LogTemp, Log, TEXT("AGM_AdventureMode::RestartGame - restarting level (not implemented)"));
	// commonly you would call UGameplayStatics::OpenLevel
}

void AGM_AdventureMode::PauseGame(bool bPause)
{
	bGamePaused = bPause;
	GetWorldSettings()->SetTimeDilation(bPause ? 0.0f : 1.0f);

	// update HUD on each local player controller if possible
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (APC_AdventureController *Adv = Cast<APC_AdventureController>(*It))
		{
			if (Adv->HUDWidget)
			{
				Adv->HUDWidget->SetPauseMenuVisible(bPause);
			}
		}
	}

	// simple AI toggle
	for (TActorIterator<AAIController> It(GetWorld()); It; ++It)
	{
		(*It)->SetPause(bPause);
	}
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
		UE_LOG(LogTemp, Log, TEXT("AGM_AdventureMode::SetGameOver - game over state entered"));
		// rudimentary input block
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (APlayerController *PC = It->Get())
			{
				PC->SetCinematicMode(true, false, false, true, true);
			}
		}
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
