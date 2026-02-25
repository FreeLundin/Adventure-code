# GASP_Update to Adventure Porting Guide

## Overview
This document outlines the camera and animation systems from GASP_Update that should be ported to the Adventure project as Blueprint assets.

---

## Phase 3: Camera/Animation Systems Port

### Status: ✅ C++ Foundation Complete, Blueprint Assets Pending

### What's Already Complete in Adventure (C++)
- ✅ **E_CameraStyle enum** (TopDown, ThirdPerson, FirstPerson) in [AdventureTypes.h](c:/Unreal_Projects/Adventure/Source/Adventure/Public/Core/AdventureTypes.h)
- ✅ **MultiPerspectiveCameraComponent** - Full C++ implementation with smooth transitions
- ✅ **Character Integration** - All character variants have camera toggle functionality
- ✅ **Enhanced Input** - Camera toggle input action configured

---

## Systems to Port from GASP_Update

### 1. Camera System Enhancements

**GASP_Update Camera Assets** (Blueprint):
- `E_CameraMode.uasset` - Additional camera mode enum (may duplicate E_CameraStyle)
- `E_CameraStyle.uasset` - Camera style enum (already exists in Adventure C++)
- `CameraAsset_SandboxCharacter.uasset` - Camera configuration data asset
- `CameraDirector_SandboxCharacter.uasset` - Camera director logic
- `CHT_CameraRig.uasset` - Camera rig hierarchy table
- `Rigs/` folder - Camera rig configurations

**Recommendation**:
- **Compare** GASP's `E_CameraMode` with Adventure's `E_CameraStyle` to determine if both are needed
- **Create** Blueprint Data Assets for camera configurations per character
  - Spring arm lengths per mode
  - FOV settings per mode
  - Camera offsets per mode
- **Create** CameraDirector Blueprint for advanced camera logic (optional - C++ component handles basics)

---

### 2. Animation Notify System

**GASP_Update Animation Notifies** (Blueprint):
```
AnimNotifies/
├── BP_AnimNotify_FoleyEvent.uasset (Base class)
├── BP_AnimNotify_FoleyEvent_Handplant_L.uasset
├── BP_AnimNotify_FoleyEvent_Handplant_R.uasset
├── BP_AnimNotify_FoleyEvent_Jump.uasset
├── BP_AnimNotify_FoleyEvent_Land.uasset
├── BP_AnimNotify_FoleyEvent_Run_L.uasset
├── BP_AnimNotify_FoleyEvent_Run_R.uasset
├── BP_AnimNotify_FoleyEvent_Scuff_L.uasset
├── BP_AnimNotify_FoleyEvent_Scuff_R.uasset
├── BP_AnimNotify_FoleyEvent_Walk_L.uasset
├── BP_AnimNotify_FoleyEvent_Walk_R.uasset
├── BP_NotifyState_EarlyTransition.uasset
├── BP_NotifyState_MontageBlendOut.uasset
├── E_EarlyTransition_Condition.uasset (Enum)
├── E_EarlyTransition_Destination.uasset (Enum)
├── E_FoleyEventSide.uasset (Enum: Left/Right)
└── E_TraversalBlendOutCondition.uasset (Enum)
```

**Implementation Steps**:
1. **Create C++ Base Classes** (Recommended):
   - `UAnimNotify_FoleyEvent` - Base notify for footstep/movement audio
   - `UAnimNotifyState_EarlyTransition` - Notify state for animation transitions
   - `UAnimNotifyState_MontageBlendOut` - Montage blending control

2. **Create Enums in AdventureTypes.h**:
   ```cpp
   UENUM(BlueprintType)
   enum class E_FoleyEventSide : uint8
   {
       Left UMETA(DisplayName = "Left"),
       Right UMETA(DisplayName = "Right")
   };
   
   UENUM(BlueprintType)
   enum class E_EarlyTransition_Condition : uint8
   {
       OnInput UMETA(DisplayName = "On Input"),
       OnStateChange UMETA(DisplayName = "On State Change"),
       OnTimer UMETA(DisplayName = "On Timer")
   };
   ```

3. **Extend in Blueprint**:
   - Create specific variants (Walk_L, Walk_R, Run_L, Run_R, Jump, Land, etc.)
   - Configure audio events per notify

---

### 3. Animation Modifier System

**GASP_Update Animation Modifiers** (Blueprint):
```
AnimModifiers/
├── AM_BakePhaseCurveFromFootstepNotifies.uasset
├── AM_Copy_IKFootRoot.uasset
├── AM_DistanceFromLedge.uasset
├── AM_FootSpeed_L.uasset
├── AM_FootSpeed_R.uasset
├── AM_FootSteps_Modulation.uasset
├── AM_FootSteps_Run.uasset
├── AM_FootSteps_Walk.uasset
├── AM_MoveData_Speed.uasset
├── AM_OrientationWarpingAlpha.uasset
├── AM_RateWarpingAlpha.uasset
├── AM_RemoveCurves.uasset
├── AM_RenameCurve.uasset
├── AM_ReorderCurves.uasset
├── AM_Reset_Attach.uasset
├── AM_TriggerWeightThreshold.uasset
└── AM_WarpingAlpha.uasset
```

**Purpose**:
- **Foot Speed Curves** - Calculate foot velocity for IK and audio modulation
- **Warping Alpha Curves** - Bake animation warping strength curves
- **Footstep Automation** - Auto-place footstep notifies based on foot speed
- **Curve Management** - Rename, remove, reorder animation curves

**Implementation**:
- These are **Animation Modifier assets** in Unreal (no C++ needed unless custom logic required)
- **Copy directly** from GASP_Update to Adventure Content folder
- Or **recreate** using UE5's Animation Modifier system in the editor

---

### 4. Movement Mode System (Mover 2.0 Integration)

**GASP_Update Movement Modes** (Blueprint):
```
MovementModes/
├── BP_MovementMode_Falling.uasset
├── BP_MovementMode_Slide.uasset
├── BP_MovementMode_Walking.uasset
├── BP_MovementTransition_FromSlide.uasset
└── BP_MovementTransition_ToSlide.uasset
```

**Purpose**:
- Custom movement modes for Mover 2.0 framework
- Slide mechanic with transitions
- Walking and falling state customization

**Implementation** (For CBP_AdventureCharacter_Mover):
1. **Option A - Blueprint Extension**:
   - Copy GASP movement mode BPs to Adventure
   - Reference them in BP_AdventureCharacter_Mover Blueprint child

2. **Option B - C++ Implementation** (Future Phase):
   - Implement `UMovementMode_Slide` C++ class
   - Implement `UMovementTransition_ToSlide` transition logic
   - Register with CharacterMoverComponent

---

### 5. Traversal Component System

**GASP_Update Traversal Components** (Blueprint):
- `AC_TraversalLogic.uasset` - Core traversal logic component
- `AC_VisualOverrideManager.uasset` - Visual state management
- `AC_PreCMCTick.uasset` - Pre-movement tick logic

**Implementation**:
- **Create C++ Components** (Recommended for performance):
  - `UTraversalLogicComponent` - Vault, mantle, climb detection
  - `UVisualOverrideManagerComponent` - Character visual state machine
  - `UPreMovementTickComponent` - Pre-CMC tick for state prep

- **Or Port as Blueprints**:
  - Copy to Adventure Content/Blueprints/Components/
  - Attach to character variants as needed

---

### 6. Smart Objects Integration

**GASP_Update Smart Objects** (Folder exists but contents unknown):
- `SmartObjects/` - Likely contains context-sensitive interaction definitions

**Recommendation**:
- Examine GASP_Update's SmartObjects folder in-editor
- Port any reusable templates (doors, ladders, ledges, etc.)

---

## Implementation Priority

### High Priority (Do First):
1. ✅ **Sandbox Character C++ Variants** - COMPLETE
2. **Animation Notifies** - Foley events for footsteps (needed for Audio team)
3. **Camera Data Assets** - Per-character camera configuration

### Medium Priority:
4. **Animation Modifiers** - Foot speed and warping curves (improves animation quality)
5. **Movement Modes** - Slide system for Mover characters (gameplay feature)

### Low Priority (Polish):
6. **Traversal Components** - Vault/mantle system (can be Blueprint-first)
7. **CameraDirector** - Advanced camera logic (optional enhancement)
8. **Smart Objects** - Context-sensitive interactions (content-driven)

---

## Next Steps

### For Blueprint Asset Porting:
1. Open GASP_Update project in Unreal Editor
2. Migrate assets using Asset Migration tool:
   - Right-click asset → Asset Actions → Migrate
   - Select Adventure project as destination
3. Verify dependencies are included
4. Test in Adventure with Sandbox characters

### For C++ Implementation:
1. Create enum definitions in [AdventureTypes.h](c:/Unreal_Projects/Adventure/Source/Adventure/Public/Core/AdventureTypes.h)
2. Create component base classes in `Source/Adventure/Public/Components/`
3. Implement core logic in `.cpp` files
4. Extend in Blueprint for content-configurable behavior

---

## Notes

- **GASP_Update has no C++ source** - All systems are Blueprint-implemented
- **Adventure hybrid approach** - Core systems in C++ (GAS, camera, movement), gameplay in Blueprint
- **Migration is non-destructive** - Original GASP_Update assets remain unchanged
- **Test incrementally** - Port one system at a time and verify compilation/functionality

---

## Questions to Resolve

1. **E_CameraMode vs E_CameraStyle**: Are these duplicates or complementary enums?
2. **Mover Integration**: Should we prioritize full Mover 2.0 implementation or keep simplified ACharacter for Sandbox variants?
3. **Audio System**: Does Adventure have MetaSounds/audio system ready for foley events?
4. **Animation Blueprint**: Do Sandbox characters need full ABPs or simplified locomotion?

---

**Last Updated**: February 24, 2026  
**Author**: GitHub Copilot  
**Related Files**:
- [CBP_SandboxCharacter_CMC.h](c:/Unreal_Projects/Adventure/Source/Adventure/Public/Character/CBP_SandboxCharacter_CMC.h)
- [CBP_SandboxCharacter_Mover.h](c:/Unreal_Projects/Adventure/Source/Adventure/Public/Character/CBP_SandboxCharacter_Mover.h)
- [AdventureTypes.h](c:/Unreal_Projects/Adventure/Source/Adventure/Public/Core/AdventureTypes.h)
- [MultiPerspectiveCameraComponent.h](c:/Unreal_Projects/Adventure/Source/Adventure/Public/Character/MultiPerspectiveCameraComponent.h)
