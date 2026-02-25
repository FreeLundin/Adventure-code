// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Core Game Mode

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/AdventureTypes.h"
#include "GM_AdventureMode.generated.h"

class APC_AdventureController;
class ACBP_AdventureCharacter;
class ACBP_AdventureCharacter_Mover;

/**
 * AGM_AdventureMode
 * 
 * Core game mode for the Adventure prototype (Phase 1+).
 * Handles:
 *   - Player pawn spawning and possession
 *   - Game state initialization and progression
 *   - Difficulty and tuning parameters for the current level
 *   - Character attribute defaults (Health, RitualEnergy, Stamina)
 *   - Traversal system configuration
 *   - Audio and VFX settings
 * 
 * Integration Points:
 *   - Works with APC_AdventureController for player input/camera setup
 *   - Initializes ACBP_AdventureCharacter with default attribute values
 *   - Integrates with GAS (Gameplay Ability System) for ability initialization
 *   - Manages level progression callbacks and quest/mission events
 * 
 * Responsibilities:
 *   - Create default game rules (friendly fire, respawning, etc.)
 *   - Provide tunable curves and data tables for locomotion, combat, and traversal
 *   - Configure HUD display options (minimal/full)
 *   - Handle player death and respawn logic
 *   - Track and broadcast game state changes
 * 
 * Usage:
 *   1. Set AGM_AdventureMode as GameMode class in Project Settings or level Blueprint
 *   2. Configure default spawning behavior and player controller class
 *   3. Tune level-specific parameters (difficulty, enemy counts, resource amounts)
 *   4. Integrate with SaveGame system for progression persistence
 * 
 * @see APC_AdventureController
 * @see ACBP_AdventureCharacter
 * @see UGASAttributeSet for attribute initialization
 */
UCLASS(Blueprintable, BlueprintType)
class ADVENTURE_API AGM_AdventureMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGM_AdventureMode();

	// ===== GAME INITIALIZATION =====

	/**
	 * BeginPlay
	 * 
	 * Called when the game mode is initialized at level start.
	 * Sets up default gameplay parameters, spawns player, and initializes HUD.
	 * Called after all actors are spawned during level transition.
	 */
	virtual void BeginPlay() override;

	/**
	 * PostLogin
	 * 
	 * Called after a player controller logs in.
	 * Used to initialize player-specific settings (camera preferences, HUD visibility, control scheme).
	 * 
	 * @param NewPlayer		The player controller that just joined
	 */
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// ===== CHARACTER & PAWN MANAGEMENT =====

	/**
	 * ReturnToMainMenu
	 * 
	 * Handles returning to main menu from gameplay.
	 * Can save current progress or clear level data as needed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void ReturnToMainMenu();

	/**
	 * RestartGame
	 * 
	 * Restarts the current level.
	 * Resets player position, clears temporary state, respawns enemies.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void RestartGame();

	/**
	 * PauseGame
	 * 
	 * Pauses gameplay by setting world time dilation.
	 * Freezes character movement and AI while allowing HUD/UI interaction.
	 * 
	 * @param bPause	True to pause, false to resume
	 */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void PauseGame(bool bPause);

	// ===== GAME STATE & PROGRESSION =====

	/**
	 * GetIsGameOver
	 * 
	 * Queries whether the game is currently in a game-over state.
	 * Used for UI flow and input blocking.
	 * 
	 * @return		True if player is dead or level is failed, false if still in progress
	 */
	UFUNCTION(BlueprintPure, Category = "Game")
	bool GetIsGameOver() const;

	/**
	 * SetGameOver
	 * 
	 * Transitions game to game-over state.
	 * Disables player input, triggers defeat VFX/audio, prepares restart or menu options.
	 * 
	 * @param bGameOver		True to trigger game over, false to resume (for debugging)
	 */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void SetGameOver(bool bGameOver);

	// ===== DIFFICULTY & TUNING =====

	/**
	 * GetDifficultyMultiplier
	 * 
	 * Returns a difficulty multiplier applied to damage, health, and ability costs.
	 * Used for balancing encounters and tuning challenge.
	 * 
	 * @return		Multiplier (1.0 = normal, 0.5 = easy, 2.0 = hard)
	 */
	UFUNCTION(BlueprintPure, Category = "Game|Difficulty")
	float GetDifficultyMultiplier() const;

	/**
	 * SetDifficulty
	 * 
	 * Sets game difficulty level (Easy, Normal, Hard).
	 * Affects all subsequent damage, health, and resource calculations.
	 * 
	 * @param NewDifficulty	Difficulty level identifier
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Difficulty")
	void SetDifficulty(int32 NewDifficulty);

	// ===== HUD & DISPLAY =====

	/**
	 * ToggleHUDVisibility
	 * 
	 * Show/hide the main gameplay HUD (health, ritual energy, ammo).
	 * Useful for cinematic sequences or clean screenshots.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|UI")
	void ToggleHUDVisibility();

	/**
	 * GetHUDVisible
	 * 
	 * Queries current HUD visibility state.
	 * 
	 * @return		True if HUD is visible, false if hidden
	 */
	UFUNCTION(BlueprintPure, Category = "Game|UI")
	bool GetHUDVisible() const;

	// ===== PLAYER CONTROLLER & CHARACTER ACCESS =====

	/**
	 * GetAdventurePlayerController
	 * 
	 * Returns the primary player controller (cast to APC_AdventureController).
	 * Provides convenient access to the custom player controller throughout gameplay.
	 * 
	 * @return		The adventure-specific player controller, or nullptr if not yet spawned
	 */
	UFUNCTION(BlueprintPure, Category = "Game")
	APC_AdventureController* GetAdventurePlayerController() const;

	/**
	 * GetAdventureCharacter
	 * 
	 * Returns the player pawn as ACBP_AdventureCharacter.
	 * Used by gameplay systems to access character state and abilities.
	 * 
	 * @return		The adventure character, or nullptr if not possessed or spawned
	 */
	UFUNCTION(BlueprintPure, Category = "Game")
	ACBP_AdventureCharacter* GetAdventureCharacter() const;

	// ===== AUDIO & FEEDBACK =====

	/**
	 * PlayGameAudioEvent
	 * 
	 * Plays a global (non-positional) game event sound.
	 * Used for music transitions, UI sounds, and announcements.
	 * 
	 * @param AudioTag		Gameplay tag identifying the audio event
	 * @param VolumeMultiplier	Volume scale applied to base event volume
	 */
	UFUNCTION(BlueprintCallable, Category = "Game|Audio")
	void PlayGameAudioEvent(FGameplayTag AudioTag, float VolumeMultiplier = 1.0f);

	// ===== GAME CONFIGURATION (Tunable) =====

	/** Multiplier applied to all incoming damage (difficulty/balance tuning) */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Game|Balance")
	float DamageMultiplier;

	/** Multiplier applied to enemy health (difficulty/balance tuning) */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Game|Balance")
	float EnemyHealthMultiplier;

	/** Multiplier applied to Ritual Energy costs for abilities (difficulty/balance tuning) */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Game|Balance")
	float AbilityCostMultiplier;

	/** Whether friendly fire is enabled (players can damage each other) */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Game|Rules")
	bool bEnableFriendlyFire;

	/** Enable or disable respawning on death */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Game|Rules")
	bool bEnableRespawn;

	/** Time (seconds) before respawn after player death */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Game|Rules")
	float RespawnDelay;

	/** Toggle HUD minimal mode (health + energy only) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game|UI")
	bool bHUDMinimalMode;

	/** Curve for Ritual Energy decay rate over time (customizable by difficulty) */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Game|Balance")
	TObjectPtr<UCurveFloat> RitualEnergyDecayCurve;

	// ===== INTERNAL STATE =====

	/** Current difficulty level (0 = Easy, 1 = Normal, 2 = Hard) */
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	int32 CurrentDifficulty;

	/** Tracks whether game is currently paused */
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	bool bGamePaused;

	/** Tracks whether game is in over state (loss/defeat) */
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	bool bGameOver;

	/** Visibility toggle for HUD elements */
	UPROPERTY(BlueprintReadOnly, Category = "Game|UI")
	bool bHUDVisible;
};
