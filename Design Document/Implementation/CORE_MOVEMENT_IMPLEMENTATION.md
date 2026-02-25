# Core Movement Implementation Guide

**Date:** Feb 21, 2026  
**Status:** Basic movement framework implemented; fine-tuning pending  
**Est. Time to Full Implementation:** 4-6 hours (animations + tweaking)  
**Key Deliverables:** Walk/Run/Sprint locomotion with input mapping

---

## Overview

**Movement System Architecture:**
- ✅ UpdateMovement_PreCMC() — Applies input-based velocity to character
- ✅ UpdateRotation_PreCMC() — Rotates character based on input direction
- ✅ GetDesiredGait() — Determines Walk/Run/Sprint based on input/ability state
- ✅ CalculateMaxSpeed() — Gait-based speed mapping with strafe curves
- ✅ CalculateMaxAcceleration() — Gait-specific acceleration values
- ✅ CalculateBrakingDeceleration() — Gait-specific deceleration (stopping)

**Input Flow:**
```
Player Input (WASD keys)
   ↓
PlayerController processes input
   ↓
Character::OnMoveInput() stores movement vector
   ↓
UpdateMovement_PreCMC() reads input, applies velocity
   ↓
UpdateRotation_PreCMC() aligns character with input direction
   ↓
CharacterMovementComponent.TickComponent() processes velocity
   ↓
Character moves in world + plays matching animation
```

---

## Implementation Status

### Completed ✅

| Component | Status | Location |
|-----------|--------|----------|
| **UpdateMovement_PreCMC()** | Basic implementation | CBP_AdventureCharacter.cpp:104-161 |
| **UpdateRotation_PreCMC()** | Basic implementation | CBP_AdventureCharacter.cpp:163-201 |
| **GetDesiredGait()** | Gait selection logic | CBP_AdventureCharacter.cpp:208-221 |
| **CalculateMaxSpeed()** | Gait-based speed mapping | CBP_AdventureCharacter.cpp:223-248 |
| **CalculateMaxAcceleration()** | Gait acceleration values | CBP_AdventureCharacter.cpp:250-260 |
| **CalculateBrakingDeceleration()** | Gait deceleration values | CBP_AdventureCharacter.cpp:262-273 |

### Pending ⏳

| Component | Issue | Blocker |
|-----------|-------|---------|
| **Animation Synchronization** | Animations not yet retargeted | Task 2 (animation source) |
| **Sprint Ability Integration** | GetDesiredGait() needs GAS query | GAS abilities not yet live |
| **Camera-Relative Movement** | Rotation doesn't consider camera angle | Camera setup incomplete |
| **Smooth Transitions** | Speed/direction changes could be smoother | Requires AnimBP interpolation |
| **Stamina Gating** | Sprint should deplete stamina | GAS attributes need wiring |

---

## Core Movement Functions

### 1. UpdateMovement_PreCMC()

**Purpose:** Pre-CMC tick hook that applies input-based velocity to character

**Algorithm:**
```
1. Guard: Only process if locally controlled (prevent server/AI conflicts)
2. Get CharacterMovementComponent
3. Read last movement input vector (WASD accumulated)
4. If no input: return (character coasts to stop via CMC friction)
5. Determine gait (Walk/Run/Sprint) → Get max speed
6. Normalize input direction
7. Calculate world-space velocity from input * max speed
8. Apply to CMC.Velocity (preserve Z for gravity/jump)
```

**Current Implementation Limitations:**
- ⚠️ Uses fixed speeds per gait; doesn't interpolate smoothly between gaits
- ⚠️ Doesn't account for slope/terrain angle
- ⚠️ Rotation happens simultaneously; could cause sliding

**Future Enhancements:**
- Add acceleration ramp-up (smooth speed transition, not instant)
- Add slope speed modifiers (uphill slower, downhill faster)
- Add friction/sliding behavior on different terrain types

### 2. UpdateRotation_PreCMC()

**Purpose:** Pre-CMC rotation that aligns character with movement input direction

**Algorithm:**
```
1. Guard: Only process if locally controlled
2. Get movement input direction
3. If no input: return (prevent continual rotation)
4. Calculate desired rotation from input direction
5. Interpolate from current rotation to desired (smooth rotation)
6. Apply new rotation to character
```

**Current Implementation Limitations:**
- ⚠️ Always rotates to face input direction (doesn't consider camera angle in third-person)
- ⚠️ Fixed rotation interpolation speed (hard-coded 10.0f)
- ⚠️ Doesn't handle first-person camera override

**Future Enhancements:**
- Respect camera style:
  - **Top-Down:** Face input direction
  - **Third-Person:** Face input direction (camera acts as reference frame)
  - **First-Person:** Face camera direction (input is relative to camera)
- Make rotation speed configurable per gait
- Add rotation prediction for smoother animations

### 3. GetDesiredGait()

**Purpose:** Determine character locomotion gait based on input and state

**Implementation:**
```cpp
if (no movement input)
    return Walk
else if (sprint ability active AND stamina > 0)
    return Sprint
else
    return Run
```

**Gait Values:**
- **Walk (0):** Safe, quiet movement for exploration/stealth
- **Run (1):** Default combat-ready movement
- **Sprint (2):** Fast movement (stamina-gated)

**Integration with GAS:** 
- TODO: Query AbilitySystemComponent for "Ability.Sprint" tag active
- TODO: Check RitualEnergy AttributeSet for stamina availability

### 4. CalculateMaxSpeed()

**Purpose:** Determine character's maximum velocity based on gait and strafe angle

**Speed Mapping:**
```
Gait: Walk  → WalkSpeeds (e.g., 300 forward, 240 strafe, 150 backward cm/s)
Gait: Run   → RunSpeeds (e.g., 600 forward, 480 strafe, 300 backward)
Gait: Sprint → SprintSpeeds (e.g., 1000 forward, 800 strafe, 500 backward)

StrafeSpeedMap (0-1): Reduces speed when moving sideways/backward
- 0.0 = Forward (uses MaxSpeed.X)
- 0.5 = Strafe (uses MaxSpeed.Y)
- 1.0 = Backward (uses MaxSpeed.Z)
```

**Configurable in Blueprint:**
```
Character defaults:
  WalkSpeeds = (300, 240, 150)
  RunSpeeds = (600, 480, 300)
  SprintSpeeds = (1000, 800, 500)
```

### 5. CalculateMaxAcceleration() & CalculateBrakingDeceleration()

**Purpose:** Determine how quickly character speeds up and slows down per gait

**Values:**
- **Walk:** Accel 1024 cm/s², Brake 2048 cm/s² (controlled)
- **Run:** Accel 2048 cm/s², Brake 2048 cm/s² (balanced)
- **Sprint:** Accel 4096 cm/s², Brake 1024 cm/s² (high momentum)

---

## Wiring to Input

### PlayerController Input Binding

Input comes from Enhanced Input System (Task 1: Create Enhanced Input assets):

```cpp
// In APC_AdventureController::SetupInputComponent()
if (UEnhancedInputComponent* EIC = GetEnhancedInputComponent())
{
    // Bind WASD / left analog stick to OnMoveInput
    EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APC_AdventureController::OnMoveInput);
}

// OnMoveInput callback:
void APC_AdventureController::OnMoveInput(const FInputActionValue& Value)
{
    if (Pawn* ControlledPawn = GetPawn())
    {
        const FVector2D MoveValue = Value.Get<FVector2D>();
        ControlledPawn->AddMovementInput(GetActorForwardVector(), MoveValue.Y);
        ControlledPawn->AddMovementInput(GetActorRightVector(), MoveValue.X);
    }
}
```

### Character Processing

Input flows to character via `ACharacter::AddMovementInput()`:

```cpp
// This accumulates input in UCharacterMovementComponent::ConsumedMovementInputVector
// PreCMC_Tick reads this value:
FVector InputDir = GetLastMovementInputVector(); // Retrieved in UpdateMovement_PreCMC()
```

---

## Testing Checklist

### Compile & Runtime
- [ ] Project compiles without errors
- [ ] Character spawns in level without crashes
- [ ] No warnings in Output Log about missing components

### Movement Behavior
- [ ] Press W → Character moves forward
- [ ] Press A/D → Character strafes left/right
- [ ] Press S → Character moves backward (slower)
- [ ] Release input → Character stops within ~2 seconds
- [ ] WASD combinations (e.g., W+A) produce diagonal movement

### Speed Testing
- [ ] Default movement speed is reasonable (not too slow, not too fast)
- [ ] Character visibly moves in VS_TestLevel (if animations retargeted)
- [ ] Walk/Run/Sprint should be noticeably different speeds (verify via console command)

### Rotation
- [ ] Character rotates to face movement direction
- [ ] Rotation is smooth (not jittery or instant)
- [ ] No spinning when input released

### Integration with GAS
- [ ] Sprint ability activation (F key) should increase speed (once Sprint ability wired)
- [ ] Stamina depletion should reduce max sprint acceleration
- [ ] Sprinting shows visual feedback (animation + reduced stamina bar)

---

## Console Commands for Debugging

```
// Enable movement debug visualization:
ShowDebug MOVEMENT

// Print character velocity:
log "CurrentVelocity: %s" GetCharacterMovement()->Velocity

// Test speed values:
GetCharacterMovement()->MaxWalkSpeed = 800.0  // Override CMC speed
```

---

## Performance Considerations

**CPU Impact:** ~0.1-0.2ms per frame
- Input processing: ~0.05ms
- Velocity calculation: ~0.05ms
- Rotation: ~0.05ms

**Optimization Tips:**
- Movement calcs are already optimized (local control check prevents server processing)
- Consider caching GetCharacterMovement() if called frequently
- Future: Use fixed timestep for deterministic movement (important for multiplayer)

---

## Animation Retargets (Blocked by Task 2)

Once animations are acquired (Task 2: Identify animation source):

### Required Animation Montages
- **Idle** — Standing idle stances per camera mode
- **Walk_Fwd/Walk_Back/Walk_Strafe** — Walk cycle animations
- **Run_Fwd/Run_Back/Run_Strafe** — Run cycle animations
- **Sprint** — Sprint forward animation
- **Jump_Start/Jump_Loop/Jump_Land** —  Airborne and landing
- **Dodge_Left/Dodge_Right/Dodge_Back** — Evasion animations
- **Transitions** — Smooth blends between gaits

### AnimBP Integration
The AnimBlueprint will:
1. Read character's Velocity
2. Query GetDesiredGait()
3. Select appropriate animation state (Idle → Walk → Run → Sprint)
4. Blend between animations smoothly
5. Apply root motion to move character (OR manual velocity application)

---

## Known Limitations & TODOs

### High Priority
- [ ] **Sprint Ability Integration:** Gait doesn't query GAS sprint status
  - Workaround: Manually set Gait in GameplayAbility
  - Fix: Add query in GetDesiredGait():  `if (ASC && ASC->GetGameplayTagCount(Ability.Sprint) > 0)`
- [ ] **Animation Missing:** Character won't animate properly without animations
  - Dependency: Task 2 (Animation Source selection + retarget)

### Medium Priority
- [ ] **Smooth Speed Transitions:** Speed changes are instant
  - Enhancement: Lerp MaxSpeed over 0.2-0.5 seconds when gait changes
- [ ] **Camera-Relative Input:** Movement always world-forward
  - Enhancement: In third-person, move relative to camera angle

### Low Priority
- [ ] **Slope Influence:** No speed modification on hills
- [ ] **Terrain Types:** Same speed on grass/stone/water
- [ ] **Footstep Sounds:** No audio integration yet (Task: Audio system)

---

## Next Steps

**Immediately After Phase 1:**
1. Complete animation retarget (Task 2) → AnimBP integration
2. Wire GAS Sprint ability → GetDesiredGait() integration
3. Add stamina gating in CalculateMaxAcceleration()
4. Fine-tune speed values based on playtesting

**Phase 2 Enhancements:**
- Smooth acceleration ramps
- Camera-relative movement (third-person)
- Slope/terrain speed modifiers
- Footstep audio
- VFX (dust trails at sprint, landing impacts)

---

## References

**Code Locations:**
- `Source/Adventure/Private/Character/CBP_AdventureCharacter.cpp` — All movement functions
- `Source/Adventure/Public/Core/AdventureTypes.h` — E_Gait, E_CameraStyle enums
- `Source/Adventure/Public/Character/CBP_AdventureCharacter.h` — Speed/acceleration properties

**Related Tasks:**
- Task 1: Enhanced Input assets (IA_Move binding)
- Task 2: Animation source (required for visible movement)
- Task 6: Camera toggle (rotation depends on camera style)

**UE5 Documentation:**
- CharacterMovement Component: https://docs.unrealengine.com/5.0/en-US/character-movement-in-unreal-engine/
- DeltaTime & Tick: https://docs.unrealengine.com/5.0/en-US/game-timing-and-framerate-in-unreal-engine/

