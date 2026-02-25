# GASP_Update → Adventure Blueprint Migration Plan

**Status**: Planning Phase  
**Date**: February 24, 2026  
**Scope**: Migrate 40+ Blueprint assets from GASP_Update to Adventure project

---

## Executive Summary

The GASP_Update project contains a comprehensive animation system with foley events, animation modifiers, movement modes, and camera systems. This migration plan outlines a phased approach to port these assets to the Adventure project for integration with the Phase 1 systems (camera, decay, HUD).

**Key Metrics**:
- **AnimNotifies**: 17 assets (11 foley events + 2 NotifyStates + 4 Enums)
- **AnimModifiers**: 17 assets
- **Movement Modes**: 5 blueprints
- **Camera System**: 5+ assets
- **Data Assets**: Structs, Curves, Enums, Helper Functions
- **Total Assets**: ~60-80 blueprints/data files

---

## Asset Inventory by Category

### 1. AnimNotify Assets (17 total)

#### Foley Event Notifies (11)
```
BP_AnimNotify_FoleyEvent.uasset                    [Base foley notify]
BP_AnimNotify_FoleyEvent_Handplant_L.uasset        [Handplant left]
BP_AnimNotify_FoleyEvent_Handplant_R.uasset        [Handplant right]
BP_AnimNotify_FoleyEvent_Jump.uasset                [Jump impact]
BP_AnimNotify_FoleyEvent_Land.uasset                [Landing impact]
BP_AnimNotify_FoleyEvent_Run_L.uasset               [Run left foot]
BP_AnimNotify_FoleyEvent_Run_R.uasset               [Run right foot]
BP_AnimNotify_FoleyEvent_Scuff_L.uasset             [Scuff left]
BP_AnimNotify_FoleyEvent_Scuff_R.uasset             [Scuff right]
BP_AnimNotify_FoleyEvent_Walk_L.uasset              [Walk left foot]
BP_AnimNotify_FoleyEvent_Walk_R.uasset              [Walk right foot]
```

**Purpose**: Trigger audio events at specific frame markers in animations  
**Integration Point**: Requires `UAnimNotify_FoleyEvent` C++ base class  
**Dependencies**: E_FoleyEventSide enum (✅ already in AdventureTypes.h)

#### NotifyState Assets (2)
```
BP_NotifyState_EarlyTransition.uasset       [Early animation transition logic]
BP_NotifyState_MontageBlendOut.uasset       [Montage blend-out behavior]
```

**Purpose**: State-based animation behaviors  
**Integration Point**: Requires `UAnimNotifyState_EarlyTransition` and `UAnimNotifyState_MontageBlendOut` base classes  
**Dependencies**: E_EarlyTransition_Condition, E_EarlyTransition_Destination, E_TraversalBlendOutCondition (✅ all in AdventureTypes.h)

#### Enum Assets (4)
```
E_FoleyEventSide.uasset                     [✅ Already in AdventureTypes.h]
E_EarlyTransition_Condition.uasset          [✅ Already in AdventureTypes.h]
E_EarlyTransition_Destination.uasset        [✅ Already in AdventureTypes.h]
E_TraversalBlendOutCondition.uasset         [✅ Already in AdventureTypes.h]
```

---

### 2. AnimModifier Assets (17 total)

#### Animation Curve Modifiers
```
AM_BakePhaseCurveFromFootstepNotifies.uasset  [Generate phase curves from foley]
AM_FootSteps_Modulation.uasset                 [Modulate footstep intervals]
AM_FootSteps_Run.uasset                        [Run footstep processing]
AM_FootSteps_Walk.uasset                       [Walk footstep processing]
AM_FootSpeed_L.uasset                          [Left foot speed curves]
AM_FootSpeed_R.uasset                          [Right foot speed curves]
AM_RenameCurve.uasset                          [Rename animation curves]
AM_RemoveCurves.uasset                         [Remove unused curves]
AM_ReorderCurves.uasset                        [Reorganize animation curves]
```

**Purpose**: Post-process animation clips with automated curve/data generation

#### Motion Warping Modifiers
```
AM_OrientationWarpingAlpha.uasset            [Orientation warping blending]
AM_RateWarpingAlpha.uasset                   [Rate warping (speed) blending]
AM_WarpingAlpha.uasset                       [Master warping alpha blending]
AM_DistanceFromLedge.uasset                  [Calculate traversal distances]
```

**Purpose**: Configure motion warping for vaulting/mantle animations

#### Utility Modifiers
```
AM_Copy_IKFootRoot.uasset                    [Copy IK to foot root curves]
AM_MoveData_Speed.uasset                     [Extract movement speed data]
AM_TriggerWeightThreshold.uasset             [Trigger montage events by weight]
AM_Reset_Attach.uasset                       [Reset attachment properties]
```

**Purpose**: General animation data management and IK support

---

### 3. MovementMode Assets (5 total)

```
BP_MovementMode_Walking.uasset               [Base walking locomotion]
BP_MovementMode_Falling.uasset               [Falling state handler]
BP_MovementMode_Slide.uasset                 [Slide traversal state]
BP_MovementTransition_ToSlide.uasset         [Transition into slide]
BP_MovementTransition_FromSlide.uasset       [Transition out of slide]
```

**Purpose**: Define character movement behaviors (walk, run, slide, fall)  
**Integration Point**: Mover 2.0 movement mode components  
**Future Work**: Port after core animation systems

---

### 4. Camera Assets (5+ total)

```
E_CameraMode.uasset                          [Camera mode enum]
E_CameraStyle.uasset                         [✅ Already in AdventureTypes.h]
CameraAsset_SandboxCharacter.uasset          [Camera configuration asset]
CameraDirector_SandboxCharacter.uasset       [Camera control logic]
CHT_CameraRig.uasset                         [Control rig for camera animation]
Rigs/                                         [Additional camera rigs]
```

**Status**: Phase 1 Camera (TopDown/ThirdPerson/FirstPerson) already implemented  
**Future Work**: Port advanced camera features if needed

---

### 5. Data Assets (35+ total)

#### Type Definitions
```
E_AnalogStickBehavior.uasset                [✅ Already in AdventureTypes.h]
E_Gait.uasset                                [✅ Already in AdventureTypes.h]
E_ExperimentalStateMachineState.uasset       [Animation state machine states]
E_MovementDirection.uasset                   [8-directional movement input]
E_MovementDirectionBias.uasset               [Movement direction bias modes]
E_MovementMode.uasset                        [Character movement modes]
E_MovementState.uasset                       [Character movement state machine]
E_RotationMode.uasset                        [Character rotation behavior]
E_Stance.uasset                              [Stance types (standing, crouching)]
E_TraversalActionType.uasset                 [Traversal action types (vault, mantle)]
```

**Action**: 4 enums already migrated, remaining 6 need to be evaluated for Adventure

#### Structs
```
S_BlendStackInputs.uasset                    [Animation blend stack configuration]
S_CharacterPropertiesForAnimation.uasset     [Animation-relevant character properties]
S_CharacterPropertiesForCamera.uasset        [Camera-relevant character properties]
S_CharacterPropertiesForTraversal.uasset     [Traversal-relevant properties]
S_ChooserOutputs.uasset                      [Chooser pattern outputs]
S_DebugGraphLineProperties.uasset            [Debug visualization properties]
S_MovementDirectionThresholds.uasset         [Movement direction input thresholds]
S_MoverCustomInputs.uasset                   [Custom Mover 2.0 inputs]
S_PlayerInputState.uasset                    [✅ Already in AdventureTypes.h as FCharacterInputState]
S_RotationOffsetCurveChooser_Inputs.uasset   [Rotation offset curve selection]
S_TraversalCheckInputs.uasset                [✅ Already in AdventureTypes.h]
S_TraversalCheckResult.uasset                [✅ Already in AdventureTypes.h]
S_TraversalChooserInputs.uasset              [Traversal action chooser inputs]
S_TraversalChooserOutputs.uasset             [Traversal action chooser outputs]
```

**Action**: 3 structs already migrated, remaining 11 need evaluation

#### Curves & Animation Data
```
CHT_RotationOffsetCurve.uasset               [Rotation offset animation curve]
Curve_RotationOffset_B.uasset                [Backward rotation offset]
Curve_RotationOffset_F.uasset                [Forward rotation offset]
Curve_RotationOffset_LL.uasset               [Left/Left diagonal offset]
Curve_RotationOffset_LR.uasset               [Left/Right diagonal offset]
Curve_RotationOffset_RL.uasset               [Right/Left diagonal offset]
Curve_RotationOffset_RR.uasset               [Right/Right diagonal offset]
Curve_RotationOffset_Slide_Knees.uasset      [Knee angle curves for slide]
Curve_StrafeSpeedMap.uasset                  [Speed mapping for strafe directions]
```

**Purpose**: Pre-calculated curves for animation blending  
**Action**: Port as-is when implementing advanced locomotion

#### Utility Assets
```
BFL_HelpfulFunctions.uasset                  [Blueprint function library]
BPI_InteractionTransform.uasset              [Interaction geometry interface]
```

---

## Migration Strategy

### Phase 1: Critical AnimNotify System (Week 1)
**Goal**: Enable foley event system and early transition logic

1. **Create C++ Base Classes** (HIGH PRIORITY)
   ```cpp
   Source/Adventure/Public/Animation/AnimNotify_FoleyEvent.h/.cpp
   Source/Adventure/Public/Animation/AnimNotifyState_EarlyTransition.h/.cpp
   Source/Adventure/Public/Animation/AnimNotifyState_MontageBlendOut.h/.cpp
   ```

2. **Migrate Foley Event Blueprints**
   - Copy from: `GASP_Update/Content/Blueprints/AnimNotifies/BP_AnimNotify_FoleyEvent*.uasset`
   - To: `Adventure/Content/Blueprints/AnimNotifies/BP_AnimNotify_FoleyEvent*.uasset`
   - Reparent to new C++ base classes

3. **Migrate NotifyState Blueprints**
   - Copy from: `GASP_Update/Content/Blueprints/AnimNotifies/BP_NotifyState_*.uasset`
   - To: `Adventure/Content/Blueprints/AnimNotifies/BP_NotifyState_*.uasset`
   - Reparent to new C++ base classes

4. **Testing**: Verify foley events trigger correctly in sandbox animations

---

### Phase 2: AnimModifier System (Week 2)
**Goal**: Implement animation data generation and motion warping

1. **Create C++ Modifier Base Classes** (if needed)
   - Evaluate if Blueprint modifiers work standalone
   - Create C++ wrapper if required

2. **Migrate AnimModifier Blueprints**
   - Copy all 17 AnimModifier assets
   - Update project path references
   - Test on sandbox character animations

3. **Documentation**: Document common animation modifier usage patterns

---

### Phase 3: Movement Modes (Week 3)
**Goal**: Port slide and advanced movement behaviors

1. **Create MovementMode Blueprints Structure**
   - Establish folder hierarchy: `Blueprints/MovementModes/`
   - Create base blueprint for movement state management

2. **Migrate Movement Mode Blueprints** (5 assets)
   - Walking, Falling, Slide modules
   - Transition logic

3. **Mover 2.0 Integration**
   - Map BlueprintNotifies to Mover movement mode transitions
   - Test slide mechanics on sandbox character

---

### Phase 4: Type System Expansion (Week 4)
**Goal**: Merge complete type ecosystem

1. **Evaluate Remaining Enums**
   - E_MovementMode, E_MovementState, E_RotationMode, E_Stance
   - E_MovementDirection, E_TraversalActionType
   - Determine which are needed for Adventure scope

2. **Merge Struct Systems**
   - S_CharacterPropertiesForAnimation, S_CharacterPropertiesForCamera, etc.
   - Add to AdventureTypes.h or separate Data file

3. **Port Curve Assets**
   - Copy rotation offset curves
   - Test in animation blueprints

---

### Phase 5: Data & Utility Assets (Week 5)
**Goal**: Complete supporting systems

1. **Migrate Helper Functions**
   - BFL_HelpfulFunctions blueprint library

2. **Port Interaction System**
   - BPI_InteractionTransform interface

3. **Documentation**: Update BP/code cross-reference guide

---

## Implementation Checklist

### Immediate (Today)
- [ ] Create AnimNotify base classes structure
- [ ] Begin Phase 1 C++ development

### Short-term (This Week)
- [ ] ✅ Sandbox character variants compiled
- [ ] Implement UAnimNotify_FoleyEvent
- [ ] Implement UAnimNotifyState_EarlyTransition
- [ ] Migrate foley event blueprints
- [ ] Test foley events in sandbox

### Medium-term (Next 2-4 weeks)
- [ ] AnimModifier system ported
- [ ] Movement modes partially integrated
- [ ] Type system expanded
- [ ] Curves migrated

### Long-term (Month+)
- [ ] Full animation system operational
- [ ] Traversal system (vault, mantle, climb)
- [ ] Advanced movement (slide, roll, etc.)
- [ ] Complete interaction system

---

## Risk Assessment

| Risk | Probability | Impact | Mitigation |
|------|-------------|--------|-----------|
| Blueprint dependencies broken during migration | Medium | High | Create mapping document, test incrementally |
| Mover 2.0 API compatibility issues | Medium | Medium | Reference Mover documentation, test on sandbox |
| Animation retargeting/LOD issues | Low | Medium | Keep GASP_Update project as reference, test thoroughly |
| Performance degradation with 40+ notifies | Low | Medium | Profile, batch register notifies, lazy-load if needed |

---

## Dependencies & Prerequisites

### Already Available
✅ E_FoleyEventSide (in AdventureTypes.h)  
✅ E_EarlyTransition_Condition (in AdventureTypes.h)  
✅ E_EarlyTransition_Destination (in AdventureTypes.h)  
✅ E_TraversalBlendOutCondition (in AdventureTypes.h)  
✅ E_CameraStyle (in AdventureTypes.h)  
✅ E_AnalogStickBehavior (in AdventureTypes.h)  
✅ E_Gait (in AdventureTypes.h)  
✅ FS_TraversalCheckInputs (in AdventureTypes.h)  
✅ FS_TraversalCheckResult (in AdventureTypes.h)  
✅ FCharacterInputState (in AdventureTypes.h)  
✅ Sandbox character variants compiled and ready  
✅ Sandbox GameMode and PlayerController with debug features  

### Need to Create
- [ ] UAnimNotify_FoleyEvent (C++ base class)
- [ ] UAnimNotifyState_EarlyTransition (C++ base class)
- [ ] UAnimNotifyState_MontageBlendOut (C++ base class)
- [ ] Enhanced animation blueprint system
- [ ] Animation modifier registration system

### External References
- GASP_Update project (reference implementation)
- Unreal Engine documentation on AnimNotifies
- Mover 2.0 plugin documentation

---

## Next Steps

1. **Create C++ AnimNotify base classes** (30 mins)
   - UAnimNotify_FoleyEvent with foley configuration
   - UAnimNotifyState_EarlyTransition with transition logic

2. **Migrate first foley blueprint** (15 mins)
   - BP_AnimNotify_FoleyEvent.uasset
   - Test parenting to C++ base class

3. **Create test animation sequence** (30 mins)
   - Add foley notify to sandbox character walking animation
   - Verify notify fires correctly

4. **Document findings** (15 mins)
   - Update migration guide with any issues found

---

## Notes

- **Enum Assets**: These are just data representations; the enums themselves are already in AdventureTypes.h. The `.uasset` files in GASP_Update are redundant and don't need migration.
- **Blueprint Depends**: Many foley/modifier/movement blueprints likely depend on each other. Migrate in logical clusters.
- **Architecture**: Consider creating a central "AnimationSystem" module to manage notifies and modifiers.
- **Performance**: With 40+ AnimNotifies, consider lazy-loading or object pooling if performance becomes an issue.

