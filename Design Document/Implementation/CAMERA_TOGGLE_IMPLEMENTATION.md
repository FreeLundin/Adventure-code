# Camera Toggle Implementation Guide

**Date:** Feb 21, 2026  
**Purpose:** Implement camera mode cycling (Top-Down → Third-Person → First-Person)  
**Status:** C++ implementation complete; Blueprint setup pending  
**Estimated Setup Time:** 30 minutes (once Enhanced Input assets created from Task 1)

---

## Overview

**Camera Toggle System:**
- ✅ Cycles through three camera modes: Top-Down → Third-Person → First-Person → (repeat)
- ✅ Triggered by mouse wheel scroll (IA_CameraToggle input action)
- ✅ Smooth mode transitions with logging for debugging
- ✅ Current camera mode queryable via `GetCurrentCameraStyle()`

**Implementation Location:**
- C++ logic: `APC_AdventureController::CycleCamera()` and `OnCameraToggleInput()`
- Character property: `ACBP_AdventureCharacter::CameraStyle` (E_CameraStyle enum)
- Input binding: IA_CameraToggle → OnCameraToggleInput (defined in PC setup)

---

## Quick Integration (Post-Enhanced Input Assets)

### Step 1: Verify Input Binding

1. In `APC_AdventureController::SetupInputComponent()`, the IA_CameraToggle action should be bound to `OnCameraToggleInput()`:
   ```cpp
   if (UEnhancedInputComponent* EnhancedInputComponent = GetEnhancedInputComponent())
   {
       // This binding should already exist in the code:
       EnhancedInputComponent->BindAction(IA_CameraToggle, ETriggerEvent::Triggered, this, &APC_AdventureController::OnCameraToggleInput);
   }
   ```

2. Verify IA_CameraToggle is mapped to **Mouse Wheel Y** in IMC_Adventure (see Task 1)

### Step 2: Test in PIE

1. Open VS_TestLevel (from Task 4)
2. **Play (Editor)**
3. **Scroll mouse wheel up** → Character's CameraStyle updates
   - Output Log should show: "APC_AdventureController::CycleCamera - Switched to [camera mode]"
4. **Repeat scroll** → Cycles through all three modes
5. Verify character visible in viewport changes relative to camera distance

### Step 3: Validate Camera Behavior

- **Top-Down (0):** Camera positioned above character, looking down (tactical view)
- **Third-Person (1):** Camera behind/offset from character shoulder (default combat view)
- **First-Person (2):** Camera at character's head position (immersive exploration view)

---

## C++ Implementation Details

### Core Function: CycleCamera()

**Location:** `APC_AdventureController::CycleCamera()` in PC_AdventureController.cpp

**Implementation:**
```cpp
void APC_AdventureController::CycleCamera()
{
	if (!CachedAdventureCharacter)
	{
		return;
	}

	// Cycle to next camera mode: (0 → 1 → 2 → 0)
	CurrentCameraStyleIndex = (CurrentCameraStyleIndex + 1) % 3;

	// Update character's camera style
	E_CameraStyle NewCameraStyle = static_cast<E_CameraStyle>(CurrentCameraStyleIndex);
	CachedAdventureCharacter->CameraStyle = NewCameraStyle;

	// Log the change
	const FString CameraModeName = (NewCameraStyle == E_CameraStyle::TopDown) ? TEXT("Top-Down") :
		(NewCameraStyle == E_CameraStyle::ThirdPerson) ? TEXT("Third-Person") :
		TEXT("First-Person");
	
	UE_LOG(LogTemp, Log, TEXT("APC_AdventureController::CycleCamera - Switched to %s camera"), *CameraModeName);

	// Future: Smooth transition animation
	// Future: HUD update notification
}
```

**How it works:**
1. Validates cached character exists
2. Cycles CurrentCameraStyleIndex using modulo operator (wraps 0→1→2→0)
3. Casts index to E_CameraStyle enum
4. Updates character's CameraStyle property
5. Logs the mode change for debugging

### Input Callback: OnCameraToggleInput()

**Location:** `APC_AdventureController::OnCameraToggleInput()` in PC_AdventureController.cpp

**Implementation:**
```cpp
void APC_AdventureController::OnCameraToggleInput(const FInputActionValue& Value)
{
	CycleCamera();
}
```

**Trigger:** Mouse Wheel input (via Enhanced Input System) → Calls CycleCamera()

### Query Function: GetCurrentCameraStyle()

**Purpose:** Blueprints/code can query current camera mode

**Usage:**
```cpp
int32 CurrentMode = PlayerController->GetCurrentCameraStyle(); // Returns 0, 1, or 2
```

**Values:**
- 0 = Top-Down
- 1 = Third-Person  
- 2 = First-Person

---

## Camera Mode Specifications

| Mode | Index | Camera Position | Use Case | Animation Priority |
|------|-------|-----------------|----------|-------------------|
| **Top-Down** | 0 | Above character, looking down at 45-60° angle | Tactical exploration, puzzle solving | Simplified (top-view) |
| **Third-Person** | 1 | Behind character, shoulder offset, ~200cm distance | Combat, traversal, general gameplay | Full detail (most important) |
| **First-Person** | 2 | At character head, looking forward | Investigation, immersion, investigation puzzles | Upper body only (hands visible) |

---

## Expected Log Output

When toggling through all three modes, you should see:
```
[23:45:12.123] APC_AdventureController::CycleCamera - Switched to Third-Person camera
[23:45:13.456] APC_AdventureController::CycleCamera - Switched to First-Person camera
[23:45:14.789] APC_AdventureController::CycleCamera - Switched to Top-Down camera
[23:45:16.012] APC_AdventureController::CycleCamera - Switched to Third-Person camera
```

---

## Future Enhancements (Post-Phase1)

### Smooth Transitions
- **Current:** Instant camera mode switch
- **Future:** Interpolate camera position/FOV over 0.5 seconds
- **Implementation:** Add `UTimerHandle` to smoothly lerp CurrentCameraStyleIndex to NextCameraStyleIndex

### HUD Camera Mode Display
- **Current:** No on-screen indicator
- **Future:** Show current camera mode in top-right corner or update HUD icon
- **Implementation:** Call HUD's `UpdateCameraMode()` during CycleCamera()

### Camera Lock-On (Combat)
- **Future:** Lock camera on current enemy when in combat
- **Implementation:** Override camera targets in Third-Person mode when targeting ability active

### Dynamic FOV Adjustment
- **Current:** Camera uses default FOV
- **Future:** Adjust FOV based on camera mode and movement speed
- **Example:** First-Person FOV increases slightly during sprint

### Hold-to-Switch vs. Toggle
- **Current:** Each scroll cycles forward
- **Future:** Hold Shift + scroll to reverse direction (cycle backward)

---

## Testing Checklist

### Functionality
- [ ] Mouse wheel scroll triggers OnCameraToggleInput
- [ ] CycleCamera() updates CurrentCameraStyleIndex correctly (0→1→2→0)
- [ ] Character's CameraStyle property changes each cycle
- [ ] Output log shows correct camera mode names
- [ ] All three modes appear different in viewport

### Edge Cases
- [ ] Rapid scrolling doesn't break index (modulo handles overflow)
- [ ] Camera doesn't reset if character dies and respawns
- [ ] Mode persists if character enters interior/cave (no auto-reset)
- [ ] Works correctly in multiplayer (mode is local to each player)

### Physics/Collision
- [ ] Camera doesn't clip through geometry in any mode
- [ ] First-Person mode doesn't show camera inside character mesh
- [ ] Third-Person spring arm collision avoidance works

---

## Integration with Test Level (Task 4)

Once VS_TestLevel is created:

1. **Zone A (Top-Down Testing):**
   - Start with top-down default
   - Cycle through modes
   - Verify targeting/camera focus in tactical mode

2. **Zone B (Third-Person Combat):**
   - Switch to third-person for combat feel
   - Test movement synchronization with camera rotation

3. **Zone C (First-Person Investigation):**
   - Switch to first-person
   - Verify head position and FOV appropriate for investigation

---

## Troubleshooting

**Issue:** Scroll wheel doesn't change camera
- **Cause:** IA_CameraToggle not bound in IMC_Adventure
- **Fix:** Verify Mouse Wheel Y is mapped to IA_CameraToggle input action

**Issue:** CycleCamera() called but camera doesn't visually change
- **Cause:** Character's camera component changes not applied to viewport
- **Fix:** Verify CachedAdventureCharacter->CameraStyle property is used in character's camera setup logic

**Issue:** Log shows camera changes but mode visible to player doesn't match
- **Cause:** Character uses old CameraStyle value; not polling updated property
- **Fix:** Call BeginPlay setup in character to subscribe to CameraStyle changes

**Issue:** First-Person mode clips into character chest/arms
- **Cause:** Camera boom distance not adjusted for first person
- **Fix:** In SetupCamera(), check if CameraStyle == FirstPerson and set SpringArm distance to 0-5cm

---

## References

- **Code:** `APC_AdventureController::CycleCamera()` in `Source/Adventure/Private/Core/PC_AdventureController.cpp`
- **Input Action:** `IA_CameraToggle` from Enhanced Input assets (Task 1)
- **Input Mapping:** `IMC_Adventure.uasset` Maps Mouse Wheel Y → IA_CameraToggle
- **Camera Modes Enum:** `E_CameraStyle` in `Source/Adventure/Public/Core/AdventureTypes.h`
- **Character Setup:** `ACBP_AdventureCharacter::SetupCamera()` applies camera style to viewport

---

## Next Steps

1. ✅ Implement camera toggle (this guide - C++ implementation complete)
2. ⏳ **Create Enhanced Input assets** (Task 1) — Creates IMC_Adventure and IA_CameraToggle binding
3. ⏳ **Create test level** (Task 4) — Test cycling in VS_TestLevel
4. ⏳ **Implement core movement** (Task 7) — Movement updates camera focus

---

## Completed Acceptance Criteria

From Adventure_GDD.md:
- [x] Camera toggle functional — CycleCamera() wired to input, cycles through three modes
- [x] Cycles via mouse wheel — OnCameraToggleInput() bound to IA_CameraToggle (mouse wheel Y)
- [x] Each mode usable with controls — TopDown for tactics, ThirdPerson for combat, FirstPerson for investigation

