// Copyright Epic Games, Inc. All Rights Reserved.
// Adventure Project - Core Player Controller

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "GameplayTagContainer.h"
#include "Core/AdventureTypes.h"
#include "PC_AdventureController.generated.h"

class ACBP_AdventureCharacter;
class ACBP_AdventureCharacter_Mover;
class UInputMappingContext;
class UInputAction;
class UUserWidget;
class UAbilitySystemComponent;

/**
 * APC_AdventureController
 * 
 * Core player controller for the Adventure prototype (Phase 1+).
 * Manages:
 *   - Player input routing and camera modes (TopDown/ThirdPerson/FirstPerson)
 *   - Enhanced Input System (EIS) setup and action callbacks
 *   - Possessed character initialization and setup
 *   - HUD widget spawning and lifetime management
 *   - GAS (Gameplay Ability System) ability activation via input
 *   - Camera state synchronization and transitions
 *   - Pause/unpause and menu navigation
 * 
 * Integration Points:
 *   - Works with ACBP_AdventureCharacter for movement and traversal
 *   - Binds input actions to character methods (movement, combat, traversal)
 *   - Manages HUD widget (WB_HUD) display and updates
 *   - Interfaces with GAS AbilitySystemComponent for ability activation
 *   - Syncs camera modes via mouse wheel or input actions
 * 
 * Input Flow:
 *   1. Player input received via Enhanced Input System
 *   2. Input actions routed to character or ability system
 *   3. Character processes movement/traversal logic
 *   4. Results updated in HUD widget via GAS attribute callbacks
 *   5. Camera follows character with style-specific offset/distance
 * 
 * Usage:
 *   1. Set APC_AdventureController as PlayerControllerClass in GameMode defaults
 *   2. Configure input mapping context and actions in Blueprint or code
 *   3. HUD widget will auto-spawn on BeginPlay
 *   4. Input setup occurs during Pawn possession (SetupInput call)
 * 
 * @see ACBP_AdventureCharacter for possession target
 * @see UInputMappingContext, UInputAction for Enhanced Input System
 * @see WB_HUD for main gameplay interface
 */
UCLASS(Blueprintable, BlueprintType)
class ADVENTURE_API APC_AdventureController : public APlayerController
{
	GENERATED_BODY()

public:
	APC_AdventureController();

	// ===== INITIALIZATION =====

	/**
	 * BeginPlay
	 * 
	 * Called when player controller enters gameplay.
	 * Spawns HUD widget and prepares input system.
	 */
	virtual void BeginPlay() override;

	/**
	 * OnPossess
	 * 
	 * Called when this controller possesses a pawn.
	 * Sets up input mapping, character components, and camera.
	 * 
	 * @param InPawn	The pawn being possessed (typically ACBP_AdventureCharacter)
	 */
	virtual void OnPossess(APawn* InPawn) override;

	/**
	 * OnUnPossess
	 * 
	 * Called when this controller loses possession of its pawn.
	 * Cleans up input mapping and cached references.
	 */
	virtual void OnUnPossess() override;

	// ===== INPUT SETUP =====

	/**
	 * SetupInputComponent
	 * 
	 * Sets up input component with Enhanced Input System callbacks.
	 * Binds movement, look, ability, and menu actions to functions.
	 * Called automatically by engine during initialization.
	 */
	virtual void SetupInputComponent() override;

	/**
	 * SetupEnhancedInput
	 * 
	 * Initializes Enhanced Input System with context and action mappings.
	 * Adds the input mapping context to the local player's input subsystem.
	 * Called from OnPossess to establish input routing.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetupEnhancedInput();

	/**
	 * TeardownEnhancedInput
	 * 
	 * Removes input mapping context when controller loses possession.
	 * Cleans up input subsystem state.
	 */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void TeardownEnhancedInput();

	/** Activate a GAS ability by tag (Sprint, Dodge, Traversal, Interact). */
	UFUNCTION(BlueprintCallable, Category = "GAS")
	void TryActivateAbilityByTag(FGameplayTag AbilityTag);

	// ===== INPUT ACTION CALLBACKS =====

	/**
	 * OnMoveInput
	 * 
	 * Callback for movement input action (WASD or analog stick).
	 * Passes input value to possessed character's movement system.
	 * 
	 * @param Value		Input action value (FInputActionValue)
	 */
	UFUNCTION()
	void OnMoveInput(const FInputActionValue& Value);

	/**
	 * OnLookInput
	 * 
	 * Callback for look input action (mouse movement or right analog stick).
	 * Updates camera rotation based on current camera style.
	 * 
	 * @param Value		Input action value (contains delta X/Y in pixels or analog units)
	 */
	UFUNCTION()
	void OnLookInput(const FInputActionValue& Value);

	/**
	 * OnSprintInput
	 * 
	 * Callback for sprint input (typically Shift key or RS pressure).
	 * Initiates sprint state on possessed character if conditions met.
	 * 
	 * @param Value		Input action value (dummy; action is binary)
	 */
	UFUNCTION()
	void OnSprintInput(const FInputActionValue& Value);

	/**
	 * OnDodgeInput
	 * 
	 * Callback for dodge/dash input (typically Space or jump key).
	 * Triggers dodge/traversal action on character.
	 * 
	 * @param Value		Input action value (dummy; action is binary or analog for prediction)
	 */
	UFUNCTION()
	void OnDodgeInput(const FInputActionValue& Value);

	/**
	 * OnAbilityInput_Light
	 * 
	 * Callback for light attack ability input (typically LMB or gamepad trigger).
	 * Activates GA_LightAttack on character's GAS ability system.
	 * 
	 * @param Value		Input action value (carry over for input predictin)
	 */
	UFUNCTION()
	void OnAbilityInput_Light(const FInputActionValue& Value);

	/**
	 * OnAbilityInput_Heavy
	 * 
	 * Callback for heavy attack ability input (typically RMB or gamepad button).
	 * Activates GA_HeavyAttack on character's GAS ability system.
	 * 
	 * @param Value		Input action value
	 */
	UFUNCTION()
	void OnAbilityInput_Heavy(const FInputActionValue& Value);

	/**
	 * OnInteractInput
	 * 
	 * Callback for interact input (typically E key).
	 * Triggers interaction on objects within range or activates triggered traversal.
	 * 
	 * @param Value		Input action value (dummy; action is binary)
	 */
	UFUNCTION()
	void OnInteractInput(const FInputActionValue& Value);

	/**
	 * OnCameraToggleInput
	 * 
	 * Callback for camera mode toggle input (typically Mouse Wheel or dedicated button).
	 * Cycles through camera styles: TopDown → ThirdPerson → FirstPerson → TopDown.
	 * 
	 * @param Value		Input action value (contains scroll delta)
	 */
	UFUNCTION()
	void OnCameraToggleInput(const FInputActionValue& Value);

	/**
	 * OnPauseInput
	 * 
	 * Callback for pause menu input (typically ESC).
	 * Opens pause menu HUD and pauses gameplay.
	 * 
	 * @param Value		Input action value (dummy; action is binary)
	 */
	UFUNCTION()
	void OnPauseInput(const FInputActionValue& Value);

	// ===== HUD & UI MANAGEMENT =====
	// (removed for baseline rollback)


	// ===== CHARACTER ACCESS =====

	/**
	 * GetAdventureCharacter
	 * 
	 * Returns the possessed pawn as ACBP_AdventureCharacter.
	 * Provides convenient typed access throughout controller lifetime.
	 * 
	 * @return		The adventure character, or nullptr if not possessing one
	 */
	UFUNCTION(BlueprintPure, Category = "Game")
	ACBP_AdventureCharacter* GetAdventureCharacter() const;

	/**
	 * GetAdventureCharacterAbilitySystem
	 * 
	 * Returns the possessed character's GAS ability system component.
	 * Used to activate abilities or query ability state.
	 * 
	 * @return		The character's AbilitySystemComponent, or nullptr if not available
	 */
	UFUNCTION(BlueprintPure, Category = "Game|GAS")
	UAbilitySystemComponent* GetAdventureCharacterAbilitySystem() const;

	// ===== GAME STATE QUERIES =====

	/**
	 * IsGamePaused
	 * 
	 * Queries current pause state.
	 * Used to block input and display appropriate UI.
	 * 
	 * @return		True if game is paused, false if running
	 */
	UFUNCTION(BlueprintPure, Category = "Game")
	bool IsGamePaused() const;

	/**
	 * SetGamePaused
	 * 
	 * Toggles game pause state.
	 * Synchronizes with game mode and HUD.
	 * 
	 * @param bPause	True to pause, false to resume
	 */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void SetGamePaused(bool bPause);

	// ===== CAMERA CONTROL =====

	/**
	 * CycleCamera
	 * 
	 * Cycles to the next camera mode (TopDown → ThirdPerson → FirstPerson → TopDown).
	 * Smoothly transitions camera position and FOV.
	 * Called by OnCameraToggleInput.
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void CycleCamera();

	/**
	 * GetCurrentCamera Style
	 * 
	 * Returns the currently active camera style.
	 * 
	 * @return		Camera style enum (TopDown, ThirdPerson, or FirstPerson)
	 */
	UFUNCTION(BlueprintPure, Category = "Camera")
	int32 GetCurrentCameraStyle() const;

	// ===== INPUT MAPPING & CONFIGURATION =====

	/** Enhanced Input System mapping context for this controller */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

	/** Input action for movement input (WASD / analog stick) */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Move;

	/** Input action for camera look input (mouse / right analog stick) */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Look;

	/** Input action for sprint/run (Shift / RS pressure) */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Sprint;

	/** Input action for dodge/jump (Space / jump button) */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Dodge;

	/** Input action for light attack ability (LMB / gamepad trigger) */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_AttackLight;

	/** Input action for heavy attack ability (RMB / gamepad button) */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_AttackHeavy;

	/** Input action for interact (E / gamepad button) */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Interact;

	/** Input action for camera toggle (Mouse Wheel / dedicated button) */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_CameraToggle;

	/** Input action for pause menu (ESC / Start button) */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Pause;

	// ===== HUD CONFIGURATION =====

	/** Main gameplay HUD widget class to spawn on BeginPlay */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	/** Cached reference to spawned HUD widget instance */
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TObjectPtr<UUserWidget> HUDWidget;

	// ===== INPUT SENSITIVITY & SETTINGS =====

	/** Base mouse/look sensitivity (default 1.0) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input|Sensitivity")
	float LookSensitivity;

	/** Invert Y axis for look input (toggle for inverted flight controls) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input|Sensitivity")
	bool bInvertYAxis;

	/** Whether gamepad deadzone is applied to analog sticks */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input|Gamepad")
	bool bUseGamepadDeadzone;

	/** Gamepad deadzone threshold (0-1, typically 0.1) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input|Gamepad", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float GamepadDeadzoneThreshold;

	// ===== INTERNAL STATE =====

	/** Cached reference to possessed adventure character */
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	TObjectPtr<ACBP_AdventureCharacter> CachedAdventureCharacter;

	/** Cached reference to possessed mover-based character */
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	TObjectPtr<ACBP_AdventureCharacter_Mover> CachedAdventureMover;

	/** Cached reference to character's GAS ability system component */
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	UAbilitySystemComponent* CachedAbilitySystemComponent;

	/** Current camera style index (0 = TopDown, 1 = ThirdPerson, 2 = FirstPerson) */
	UPROPERTY(BlueprintReadOnly, Category = "Camera")
	int32 CurrentCameraStyleIndex;

	/** Whether game is currently paused */
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	bool bGamePaused;
};
