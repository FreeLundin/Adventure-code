# PH1-CAM-001: Fix Camera System Duplicate Components in Sandbox Characters

**Owner:** Dev Team  
**Priority:** High  
**Phase:** Phase 1  
**Status:** In Progress → Done  
**Estimate:** 4-6 hours  
**Related Issue:** Gameplay camera broken post-testing (reported in session)

## Description

The Sandbox character variants (`CBP_SandboxCharacter_CMC` and `CBP_SandboxCharacter_Mover`) were inheriting from `ACharacter` instead of `ACBP_AdventureCharacter`. This caused duplicate creation of:
- `SpringArmComponent`
- `CameraComponent`
- `UMultiPerspectiveCameraComponent`
- `URitualEnergyManagerComponent`

The duplicate components caused initialization conflicts, breaking the multi-perspective camera system in sandbox gameplay.

## Root Cause Analysis

- Child classes (`ACharacter` = base pawn) created their own component instances
- Parent class (`ACBP_AdventureCharacter`) created the same components
- Result: Two sets of conflicting components with different initialization states
- Symptom: Camera not responding in-game; CameraManager/CameraToggle silent failures

## Acceptance Criteria

✅ Remove duplicate component creation by fixing inheritance hierarchy  
✅ Sandbox characters inherit from `ACBP_AdventureCharacter` instead of `ACharacter`  
✅ Add public accessor methods `GetCameraManager()` and `GetRitualEnergyManager()` to parent  
✅ Remove redundant GAS initialization code from child implementations  
✅ Project builds without C2509/UHT errors  
✅ Editor launches without access violation crashes  
✅ Camera modes toggle correctly in sandbox gameplay  
✅ Ritual energy system displays correctly in HUD  

## Implementation Approach

1. **Inheritance Refactor**
   - Change `CBP_SandboxCharacter_CMC : public ACharacter` → `: public ACBP_AdventureCharacter`
   - Change `CBP_SandboxCharacter_Mover : public ACharacter` → `: public ACBP_AdventureCharacter`

2. **Remove Duplicate Declarations**
   - Remove duplicate member variables for: `GameplayCamera`, `CameraManager`, `RitualEnergyManager`, `AbilitySystemComponent`
   - These are now inherited from parent

3. **Remove Duplicate Methods**
   - Remove: `PossessedBy()`, `OnRep_PlayerState()`, `GetAbilitySystemComponent()`, `InitializeAbilitySystem()`, `ToggleCameraMode()`
   - These are now inherited and properly implemented in parent

4. **Add Public Accessors to Parent**
   ```cpp
   UFUNCTION(BlueprintPure, Category = "Camera")
   UMultiPerspectiveCameraComponent* GetCameraManager() const;
   
   UFUNCTION(BlueprintPure, Category = "Rituals")
   URitualEnergyManagerComponent* GetRitualEnergyManager() const;
   ```

5. **Fix Initialization Timing**
   - Remove premature `SetReplicates()` calls in child constructors
   - Parent handles replication setup in `PostInitializeComponents()`

## Testing Steps

1. Build Adventure Win64 Development → 0 errors
2. Open editor → no access violation crashes
3. Play Sandbox level → character spawns
4. Verify camera modes toggle (keyboard input or UI)
5. Verify ritual energy bar updates/displays
6. Check no log warnings about missing components

## Technical Notes

**Why this fix works:**
- Single source of truth for component creation (parent class)
- Guaranteed initialization order and timing
- Child classes can extend (Mover-specific movement, CMC-specific parameters) without shadowing parent
- GAS initialization happens once, not twice

**Impact:**
- Sandbox characters now have feature parity through inheritance
- Cleaner architecture with no shadowed member variables
- Fixes camera initialization race conditions

## See Also

- [ADR-001: Sandbox Character Inheritance Architecture](../ARCHITECTURE_DECISIONS.md#adr-001)
- [Phase 1 Camera Implementation](../CAMERA_TOGGLE_IMPLEMENTATION.md)
- Parent class: `ACBP_AdventureCharacter.h`

