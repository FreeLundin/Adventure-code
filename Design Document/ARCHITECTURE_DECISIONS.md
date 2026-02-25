# Architecture Decisions — Adventure

## ADR-001: Sandbox Character Inheritance Architecture

**Status:** Accepted  
**Date:** 2026-02-24  
**Context:** Phase 1 Sandbox Mode Implementation  
**Decision Owner:** Dev Team  

### Context

During Phase 1 testing, the gameplay camera system failed in Sandbox mode. Root cause investigation revealed:

- `CBP_SandboxCharacter_CMC` and `CBP_SandboxCharacter_Mover` were extending `ACharacter` directly
- Both inherited class AND parent class were creating identical component instances:
  - `SpringArmComponent`
  - `CameraComponent` (GameplayCamera)
  - `UMultiPerspectiveCameraComponent` (CameraManager)
  - `URitualEnergyManagerComponent`
- This double-creation caused initialization race conditions and object lifecycle conflicts
- Result: Camera system silent failures, HUD ritual energy display broken

### Decision

**Sandbox character variants SHALL inherit from `ACBP_AdventureCharacter` instead of `ACharacter`.**

This ensures:
1. Single source of truth for shared components
2. Guaranteed initialization order and timing
3. No member variable shadowing
4. Feature parity achieved through composition, not duplication

### Architecture Pattern

```
ACharacter (Unreal base)
    ↓
ACBP_AdventureCharacter (Phase 1 core character)
    • SpringArm
    • GameplayCamera
    • CameraManager
    • RitualEnergyManager
    • GAS initialization (ASC on PlayerState)
    ↓
CBP_SandboxCharacter_CMC (Sandbox variant A)
    + Movement parameter tuning only
    (NO duplicate component creation)

CBP_SandboxCharacter_Mover (Sandbox variant B)
    + Mover-specific traversal hooks
    (NO duplicate component creation)
```

### Rationale

| Aspect | Inheritance from ACharacter | Inheritance from ACBP_AdventureCharacter |
|--------|---------------------------|----------------------------------------|
| Component Duplication | ❌ Yes (both create SpringArm, etc.) | ✅ No (only parent creates) |
| Initialization Timing | ❌ Race conditions | ✅ Deterministic |
| Member Shadowing | ❌ High (child redeclares parent vars) | ✅ None |
| Code Reuse | ❌ Duplicate GAS/Camera logic | ✅ Proper inheritance |
| Testing Complexity | ❌ Two camera implementations to maintain | ✅ Single camera implementation |
| Camera Reliability | ❌ Broken (duplicate initialization) | ✅ Reliable (single initialization) |

### Implementation Details

1. **Remove Duplication:**
   ```cpp
   // ❌ WRONG (Old)
   class CBP_SandboxCharacter_CMC : public ACharacter {
       USpringArmComponent* SpringArm;  // DUPLICATE!
       UCameraComponent* GameplayCamera;  // DUPLICATE!
   };
   
   // ✅ CORRECT (New)
   class CBP_SandboxCharacter_CMC : public ACBP_AdventureCharacter {
       // SpringArm, GameplayCamera inherited from parent
   };
   ```

2. **Add Accessor Methods to Parent:**
   ```cpp
   UFUNCTION(BlueprintPure, Category = "Camera")
   UMultiPerspectiveCameraComponent* GetCameraManager() const;
   
   UFUNCTION(BlueprintPure, Category = "Rituals")
   URitualEnergyManagerComponent* GetRitualEnergyManager() const;
   ```

3. **Sandbox Implementations Stay Lightweight:**
   ```cpp
   // CBP_SandboxCharacter_CMC only handles CMC-specific tuning
   ACBP_SandboxCharacter_CMC::ACBP_SandboxCharacter_CMC() {
       // Just configure movement speeds, camera parameters
       // ALL component creation handled by parent
   }
   
   // CBP_SandboxCharacter_Mover only handles Mover-specific setup
   ACBP_SandboxCharacter_Mover::ACBP_SandboxCharacter_Mover() {
       // Just configure for Mover component integration
       // ALL shared components inherited
   }
   ```

### Consequences

**Positive:**
- ✅ Fixes camera initialization bug in sandbox mode
- ✅ Eliminates redundant code (single GAS init, single camera setup)
- ✅ Easier to maintain—changes to parent benefit all variants
- ✅ Clearer test coverage (test camera once, works everywhere)
- ✅ Foundation for future character variants (Adventure Mover, Boss, etc.)

**Negative:**
- Sandbox variants less "independent" (but this is actually good—they shouldn't be)
- Requires understanding of parent class design (mitigated by documentation)

### Alternatives Considered

1. **Keep separate inheritance, fix through delegation** (rejected: more complex, still duplicates)
2. **Create unrelated component factory** (rejected: over-engineered for Phase 1)
3. **Disable child components at runtime** (rejected: wasteful, doesn't fix initialization race)

### Related Issues

- Issue: [PH1-CAM-001](./issues/PH1-CAM-001.md) (Camera System Duplicate Components)
- Parent Class: [ACBP_AdventureCharacter.h](../Source/Adventure/Public/Character/CBP_AdventureCharacter.h)
- Sandbox CMC: [CBP_SandboxCharacter_CMC.h](../Source/Adventure/Public/Character/CBP_SandboxCharacter_CMC.h)
- Sandbox Mover: [CBP_SandboxCharacter_Mover.h](../Source/Adventure/Public/Character/CBP_SandboxCharacter_Mover.h)

### References

- "Unreal C++ Architecture: Composition vs. Inheritance" (Internal best practices)
- UObject memory management & component lifecycle
- GAS documentation on ASC ownership patterns

---

**Approved by:** Dev Team  
**Last Updated:** 2026-02-24

