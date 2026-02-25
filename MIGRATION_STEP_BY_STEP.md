# Blueprint Migration - Step-by-Step Guide

**Status**: Phase 1 Complete - Blueprints Migrated  
**Date**: February 24, 2026  
**Assets Migrated**: 59 blueprint files

---

## Migration Summary

### ✅ Completed
- 17 AnimNotify blueprints → `Content/Blueprints/AnimNotifies/`
- 17 AnimModifier blueprints → `Content/Blueprints/AnimModifiers/`
- 5 MovementMode blueprints → `Content/Blueprints/MovementModes/`
- 20 Camera blueprints → `Content/Blueprints/Cameras/`

**Total**: 59 blueprint assets successfully copied from GASP_Update

---

## Next Steps: Blueprint Reparenting & Configuration

### Step 1: Update Foley Event Notifies (Priority: HIGH)

**Goal**: Reparent foley blueprints to new C++ base class

1. In Unreal Editor, navigate to `Content/Blueprints/AnimNotifies/`
2. Open `BP_AnimNotify_FoleyEvent.uasset`
3. In Blueprint editor, go to **Class Settings**
4. Change **Parent Class** from `AnimNotify` to `AnimNotify_FoleyEvent` (C++)
5. Compile and Save
6. Repeat for remaining foley notifies:
   - `BP_AnimNotify_FoleyEvent_Handplant_L`
   - `BP_AnimNotify_FoleyEvent_Handplant_R`
   - `BP_AnimNotify_FoleyEvent_Jump`
   - `BP_AnimNotify_FoleyEvent_Land`
   - `BP_AnimNotify_FoleyEvent_Run_L`
   - `BP_AnimNotify_FoleyEvent_Run_R`
   - `BP_AnimNotify_FoleyEvent_Scuff_L`
   - `BP_AnimNotify_FoleyEvent_Scuff_R`
   - `BP_AnimNotify_FoleyEvent_Walk_L`
   - `BP_AnimNotify_FoleyEvent_Walk_R`

---

### Step 2: Update Early Transition Notifies (Priority: HIGH)

**Goal**: Reparent transition NotifyState to C++ base class

1. Open `BP_NotifyState_EarlyTransition.uasset` in `Content/Blueprints/AnimNotifies/`
2. In Blueprint editor, go to **Class Settings**
3. Change **Parent Class** to `AnimNotifyState_EarlyTransition` (C++)
4. Compile and Save
5. Verify all transition settings are preserved

---

### Step 3: Update Montage Blend-Out Notifies (Priority: HIGH)

**Goal**: Reparent blend-out NotifyState to C++ base class

1. Open `BP_NotifyState_MontageBlendOut.uasset` in `Content/Blueprints/AnimNotifies/`
2. Change **Parent Class** to `AnimNotifyState_MontageBlendOut` (C++)
3. Compile and Save

---

### Step 4: Test Notifies in Sandbox (Priority: MEDIUM)

**Goal**: Validate foley events work in sandbox animations

1. Open any animation in `Content/Characters/Sandbox/` that has foley notifies
2. Play animation in viewport
3. Verify foley sounds play at correct frame markers
4. Check debug logs for any issues:
   ```
   [LogTemp] AnimNotify_FoleyEvent: Playing foley [SoundName] at side [Left] with volume 1.0, pitch 1.0
   ```

---

### Step 5: Configure AnimModifier Blueprints (Priority: MEDIUM)

**Goal**: Verify AnimModifier blueprints function correctly

1. Open `Content/Blueprints/AnimModifiers/` folder
2. Select random modifier (e.g., `AM_FootSpeed_L.uasset`)
3. Check that all properties are intact
4. No reparenting needed - AnimModifiers typically remain as blueprints
5. Document any custom modifier logic for reference

**Key Modifiers to Verify**:
- `AM_FootSteps_Walk.uasset` - Walk animation processing
- `AM_FootSteps_Run.uasset` - Run animation processing
- `AM_OrientationWarpingAlpha.uasset` - Rotation warping
- `AM_RateWarpingAlpha.uasset` - Speed warping
- `AM_WarpingAlpha.uasset` - Master warping control

---

### Step 6: Test Movement Mode Blueprints (Priority: MEDIUM)

**Goal**: Validate movement mode states function with Mover 2.0

1. Open `Content/Blueprints/MovementModes/` folder
2. Examine each movement mode blueprint:
   - `BP_MovementMode_Walking.uasset`
   - `BP_MovementMode_Falling.uasset`
   - `BP_MovementMode_Slide.uasset`
   - `BP_MovementTransition_ToSlide.uasset`
   - `BP_MovementTransition_FromSlide.uasset`

3. Verify Mover 2.0 plugin compatibility
4. Update any plugin path references if needed

---

### Step 7: Camera Configuration (Priority: LOW)

**Goal**: Integrate GASP_Update camera settings with Phase 1 camera

1. Open `Content/Blueprints/Cameras/` folder
2. Review camera configuration assets:
   - `CameraAsset_SandboxCharacter.uasset`
   - `CameraDirector_SandboxCharacter.uasset`
   - `CHT_CameraRig.uasset`
   - Camera rigs in `Rigs/` subfolder

3. **Note**: Adventure already has `MultiPerspectiveCameraComponent` for TopDown/ThirdPerson/FirstPerson
4. Consider these GASP camera assets as reference implementations
5. Can be integrated later for advanced features

---

## Troubleshooting Guide

### Issue: "Parent class not found" error when reparenting

**Solution**:
1. Ensure C++ classes are compiled
2. Restart Unreal Editor
3. Delete Intermediate/Binaries folders if errors persist
4. Rebuild C++ project

### Issue: Foley sounds don't play

**Solution**:
1. Check C++ `PlayFoleySound()` method is being called
2. Verify sound assets exist in GASP_Update
3. Enable debug logging: Set `bDebugLogging = true` in notify properties
4. Check audio device is working in OS

### Issue: Blueprint dependencies fail to resolve

**Solution**:
1. These are expected warnings during initial migration
2. Rebuild the project: **Tools → Rebuild**
3. Many GASP_Update specific references will self-resolve
4. Any broken references will show as red in content browser

---

## Validation Checklist

**Post-Migration Verification**:

- [ ] All 17 AnimNotify blueprints load without errors
- [ ] All 17 AnimModifier blueprints load without errors
- [ ] All 5 MovementMode blueprints load without errors
- [ ] All 20 Camera blueprints load without errors
- [ ] No red errors in Content Browser
- [ ] No "missing parent class" warnings
- [ ] Foley notifies play sounds in sandbox
- [ ] Early transition notifies trigger correctly
- [ ] Montage blend-out works as expected

---

## Performance Notes

**Blueprint Count**: 59 assets  
**Estimated Load Time**: <5 seconds  
**Memory Impact**: ~50-100 MB (typical)

If performance issues occur:
1. Check for circular dependencies in blueprints
2. Profile with **Window → Developer Tools → Class Viewer**
3. Consider lazy-loading rarely-used blueprints

---

## Next Phase: Animation System Integration

Once blueprints are validated:

1. **Create test animation sequences** with foley notifies
2. **Bind foley events** to character audio component
3. **Test slide mechanic** with movement mode blueprints
4. **Integrate warping alphas** into locomotion state machine
5. **Document animation blueprint workflow**

---

## Files Reference

**C++ Base Classes** (Created in source code):
- `Source/Adventure/Public/Animation/AnimNotify_FoleyEvent.h/.cpp`
- `Source/Adventure/Public/Animation/AnimNotifyState_EarlyTransition.h/.cpp`
- `Source/Adventure/Public/Animation/AnimNotifyState_MontageBlendOut.h/.cpp`

**Migrated Blueprints**: 
- `Content/Blueprints/AnimNotifies/` (17 assets)
- `Content/Blueprints/AnimModifiers/` (17 assets)
- `Content/Blueprints/MovementModes/` (5 assets)
- `Content/Blueprints/Cameras/` (20 assets)

**Original Source**:
- `C:\Unreal_Projects\GASP_Update\Content\Blueprints\`

---

## Timeline Estimate

| Task | Time | Priority |
|------|------|----------|
| Reparent foley notifies (11 BPs) | 30 mins | HIGH |
| Reparent transition notifies (2 BPs) | 10 mins | HIGH |
| Test foley in sandbox | 15 mins | HIGH |
| Configure modifiers | 20 mins | MEDIUM |
| Test movement modes | 20 mins | MEDIUM |
| Camera integration review | 15 mins | LOW |
| Total | ~110 mins | - |

---

## Success Criteria

✅ **Phase migrated** when:
- All 59 blueprints load in content browser
- No error messages in editor logs
- Foley events trigger with audio
- Movement modes integrate with character
- Full build compiles without errors

