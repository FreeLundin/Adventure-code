# Phase 1 Completion Runbook
**Adventure | UE 5.7 | Feb 21, 2026**

---

## Objective
**Complete Phase 1 setup to achieve first playable smoke test.**

**Estimated Time:** 2-3 hours (if animations available)  
**Deliverable:** Project that compiles, spawns character, and responds to input

---

## Prerequisites Checklist

Before starting, verify:
- [ ] Project compiles successfully (Live Coding working)
- [ ] All Phase 1 code implemented (GAS foundation, movement, HUD, camera)
- [ ] VS Code IntelliSense optimized ✅
- [ ] startup performance optimized ✅
- [ ] Live Coding configured ✅

---

## PHASE 1A: INPUT SYSTEM SETUP (30 min)

### Step 1.1: Generate Enhanced Input Assets

**Location:** Editor Console

```
1. Open Editor
   File → Open Project → Adventure.uproject

2. Open Python Console
   Tools → Python Console

3. Execute asset creation script:
   import runpy
   runpy.run_path("C:/Unreal_Projects/Adventure/scripts/create_enhanced_input_assets.py")

4. Wait for completion message (should take 10-30 seconds)
   ✅ Should see: "Created IMC_Adventure" + "Created IA_*" actions

5. Verify in Content Browser
   - Navigate to Content/Input/
   - Should see:
     * IMC_Adventure (MappingContext)
     * IA_Move (Action)
     * IA_Look (Action)
     * IA_Sprint (Action)
     * IA_Dodge (Action)
     * IA_LightAttack (Action)
     * IA_HeavyAttack (Action)
     * IA_Interact (Action)
     * IA_Parry (Action)
     * IA_CameraToggle (Action)
```

**If Script Fails:**
```
Fallback: Manual Creation in 5 minutes
1. Right-click in Content/Input/
2. New → Input MappingContext → Rename to IMC_Adventure
3. Repeat for each IA_* action
4. Save all
```

### Step 1.2: Bind Keys in IMC_Adventure

**Location:** Content/Input/IMC_Adventure

```
1. Open IMC_Adventure in Editor
   Double-click in Content Browser

2. For each action, add key binding:

   ACTION              KEY         VALUE
   ────────────────────────────────────────
   IA_Move             W/A/S/D     Value: WASD
   IA_Look             Mouse       Value: Mouse X/Y
   IA_Sprint           Shift       Value: 1.0
   IA_Dodge            Space       Value: 1.0
   IA_LightAttack      Left Mouse  Value: 1.0
   IA_HeavyAttack      Right Mouse Value: 1.0
   IA_Interact         E           Value: 1.0
   IA_Parry            Q           Value: 1.0
   IA_CameraToggle     Mouse Wheel Value: +1.0/-1.0

3. Save (Ctrl+S)
```

**Quick Reference for Key Names:**
- WASD: `W Key`, `A Key`, `S Key`, `D Key`
- Mouse: `Mouse X`, `Mouse Y`
- Shift: `Left Shift`
- Space: `Spacebar`
- LMB/RMB: `Left Mouse Button`, `Right Mouse Button`
- Scroll Up/Down: `Mouse Wheel Up`, `Mouse Wheel Down`
- E, Q: `E Key`, `Q Key`

### Step 1.3: Verify Input Binding in PC_AdventureController

**Check:** `Source/Adventure/Private/Core/PC_AdventureController.cpp`

Verify function exists:
```cpp
void APC_AdventureController::SpawnHUD()
{
    // Should initialize HUD + set up input mapping
    if (APC_AdventureController* OwnerController = Cast<APC_AdventureController>(Owner))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
            GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            Subsystem->AddMappingContext(IMC_Adventure, 0);
        }
    }
}
```

✅ **Input System Complete**

---

## PHASE 1B: TEST LEVEL SETUP (20 min)

### Step 2.1: Create Test Level

**Location:** Content/Levels/

```
1. Editor → Main Menu
   File → New Level

2. Choose Default Blank Level
   (NOT with landscape, NOT with obstacles yet)

3. Save Level
   Ctrl+S → Save As → Content/Levels/VS_TestLevel

4. Add Basic Geometry
   - Floor plane: 10,000 x 10,000 cm
   - Walls: Optional (just for visual reference)
   
   Method:
   a) Place → Basic → Cube
   b) Scale: 100 x 100 x 1 (floor)
   c) Position: Z = -50 (floor below spawn)
   d) Repeat for walls if desired

5. Set Player Start
   Place → Basic → Player Start (one in center)

6. Set Default Pawn
   World Settings (top right) → GameMode Override
   → Game Mode Base → Select GM_AdventureMode

7. Save (Ctrl+S)
```

### Step 2.2: Configure Level as Default Startup Map

**Location:** Project Settings

```
1. Edit → Project Settings
   (or Config/DefaultEngine.ini)

2. Search: "Maps"
   /Script/EngineSettings.GameMapsSettings

3. Set BOTH:
   - Editor Startup Map: /Game/Levels/VS_TestLevel
   - Game Default Map: /Game/Levels/VS_TestLevel

4. Save & close
```

✅ **Test Level Complete**

---

## PHASE 1C: HUD WIDGET SETUP (30 min)

### Step 3.1: Create HUD Blueprint Child Class

**Location:** Content/UI/

```
1. Editor Content Browser
   Right-click → Blueprint Class

2. Choose Parent Class
   Search "WB_HUD" → Select it

3. Rename: BP_HUD_Main

4. Open Blueprint (double-click)

5. Designer Tab (Visual Layout)
   
   Create hierarchy:
   
   Canvas Panel (root)
   ├── HealthBar (ProgressBar)
   │   ├── Min: 0
   │   ├── Max: 100
   │   ├── Start Value: 100
   │   └── Color: Red gradient
   ├── StaminaBar (ProgressBar)
   │   ├── Min: 0
   │   ├── Max: 100
   │   ├── Start Value: 100
   │   └── Color: Green gradient
   └── RitualEnergyBar (ProgressBar)
       ├── Min: 0
       ├── Max: 100
       ├── Start Value: 0
       └── Color: Purple gradient

6. Position bars in top-left (0, 20, 0) with 2px spacing

7. Compile & Save (Ctrl+S)
```

### Step 3.2: Wire Up HUD to Character

**In Editor → VS_TestLevel:**

```
1. Place character in level:
   Place → Search "ACBP_AdventureCharacter_Mover"
   (drops one character instance in level center)

2. Verify in Details: 
   - Use Controller Rotation Yaw: ON
   - Use Controller Rotation Pitch: ON

3. Play in Editor (Alt+P)

4. Check HUD appears (top-left screen)
   - Should see 3 bars (health red, stamina green, ritual purple)

5. Stop (Esc)
```

✅ **HUD Setup Complete**

---

## PHASE 1D: ANIMATION DECISION (Choose Path)

### Step 4.1: Evaluate Options

| Option | Time | Scope | Recommendation |
|--------|------|-------|-----------------|
| **Mannequin** | 7-9 hours | Core Walk/Run/Sprint montages | ✅ **Phase 1** |
| **ALS v4** | 15-20 hours | Full animation suite | **Phase 2+** |

### Step 4.2: Decision Point

**Choose ONE:**

```
□ Option A: Mannequin (7-9 hours)
  - Export Mannequin skeleton from Engine content
  - Retarget core animations (Walk, Run, Sprint, Idle)
  - Implement in movement system
  - Ready for playtest
  
□ Option B: ALS v4 (15-20 hours)
  - Download ALS v4 from Marketplace
  - Import all animations
  - Retarget to project skeleton
  - More comprehensive but slower

□ Option C: Placeholder (No animations)
  - Use root motion only (character moves but doesn't animate)
  - Fast for testing movement logic
  - Add animations later
```

**Recommended:** Option A (Mannequin) for rapid Phase 1 completion

### Step 4.3: Execute Animation Path

**If Mannequin Path:**

```
1. Export Engine Mannequin Animations
   - Engine Content → ThirdPersonBP → Mannequin
   - Right-click each montage → Asset Actions → Migrate
   - Copy to: Content/Characters/Mannequin/Animations/

2. Retarget to Adventure Skeleton
   - Skeleton Editor → Retarget Skeleton
   - Map bones (Mannequin → Adventure character)
   - Save retargeted animations

3. Create Animation Blueprint
   - Right-click → Blueprint Class (Animation Blueprint)
   - Parent: AnimInstance
   - Name: ABP_AdventureCharacter
   - Wire movement state to montages

4. Assign to Character
   - ACBP_AdventureCharacter Blueprint
   - Details → Mesh → Anim Class = ABP_AdventureCharacter

5. Test in VS_TestLevel
   - Play → Move with WASD → Should animate
```

**If No Animations Yet:**

```
1. Character will move without animation (root motion only)
2. Proceed to Smoke Test
3. Add animations after Phase 1 setup validated
```

⏸️ **Animation Setup Pending (Choose & Execute)**

---

## PHASE 1E: SMOKE TEST (15 min)

### Step 5.1: Pre-Test Checklist

Before playing, verify:
- [ ] Project compiles (no code errors)
- [ ] Enhanced Input assets created ✅
- [ ] Keys bound in IMC_Adventure ✅
- [ ] VS_TestLevel created & set as default ✅
- [ ] HUD Blueprint child created ✅
- [ ] Character (Mover variant) in level ✅
- [ ] Animations assigned (if available) ⏸️

### Step 5.2: Run Smoke Test

**Launch Game:**

```
1. Editor → Play (Alt+P)

2. Test Input:
   ✅ W/A/S/D → Character moves
   ✅ Shift → Character sprints (if animations present)
   ✅ Mouse Wheel → Camera cycles (Top-Down → Third → First)
   ✅ Mouse Movement → Camera rotates

3. Test HUD:
   ✅ Health bar visible (top-left)
   ✅ Stamina bar visible
   ✅ Ritual Energy bar visible

4. Verify Output Log:
   - No compile errors in editor
   - No warnings about missing input mappings
   - No GAS attribute errors

5. Exit (Esc)
```

### Step 5.3: Test Result Evaluation

**Passing Smoke Test:**
```
✅ All of above working → PROCEED TO COMPLETION
❌ Input not responding → Check IMC_Adventure key bindings
❌ Character not visible → Check PlayerStart placement
❌ HUD not showing → Check WB_HUD initialization in PC
❌ Crash on startup → Check GAS attribute setup
```

---

## PHASE 1F: COMPLETION & COMMIT

### Step 6.1: Final Validation

Run full checklist:

```
Smoke Test Results:
☐ Project compiles (no errors)
☐ Character spawns in test level
☐ WASD input moves character
☐ Camera toggle works (Mouse Wheel)
☐ HUD displays on screen
☐ No runtime errors in Output Log
```

### Step 6.2: Git Commit

**Commit all Phase 1 setup:**

```powershell
cd c:\Unreal_Projects\Adventure

# Stage all changes
git add -A

# Commit with descriptive message
git commit -m "feat(phase1): complete setup for playtest - input system, HUD, test level, and smoke test passing"

# View commit
git log --oneline -1
```

### Step 6.3: Tag Milestone

```powershell
# Mark Phase 1 completion
git tag -a "phase1-complete" -m "Phase 1: Core gameplay framework ready for playtest"

# Verify
git tag -l
```

---

## Timeline & Estimates

| Task | Est. Time | Status |
|------|-----------|--------|
| 1.1: Enhanced Input Script | 5 min | ⏳ |
| 1.2: Bind Keys | 15 min | ⏳ |
| 1.3: Verify Input | 5 min | ⏳ |
| **SUBTOTAL PHASE 1A** | **25 min** | |
| 2.1: Create Test Level | 15 min | ⏳ |
| 2.2: Set Default Map | 5 min | ⏳ |
| **SUBTOTAL PHASE 1B** | **20 min** | |
| 3.1: Create HUD Blueprint | 20 min | ⏳ |
| 3.2: Wire to Character | 10 min | ⏳ |
| **SUBTOTAL PHASE 1C** | **30 min** | |
| 4: Animation Decision & Setup | 0-20 min | ⏸️ PENDING |
| 5: Smoke Test & Validation | 15 min | ⏳ |
| 6: Git Commit & Tag | 5 min | ⏳ |
| **TOTAL (NO ANIMATIONS)** | **95 min (1.6 hrs)** | |
| **TOTAL (WITH MANNEQUIN)** | **185 min (3.1 hrs)** | |

---

## Success Criteria

**Phase 1 Complete When:**

1. ✅ All code compiles with zero errors
2. ✅ Smoke test passes (all checks above)
3. ✅ Character responds to WASD input
4. ✅ Camera toggles with mouse wheel
5. ✅ HUD displays health/stamina/ritual energy
6. ✅ No runtime crashes or exceptions
7. ✅ Git tag `phase1-complete` created

---

## Troubleshooting Quick Reference

| Issue | Fix |
|-------|-----|
| Input not responding | Verify IMC_Adventure bound in PlayerController → SpawnHUD() |
| Character not moving | Check WASD keys in IMC_Adventure; verify character blueprint assigned |
| HUD not visible | Check UWB_HUD initialization; verify AddToViewport() called |
| Camera won't toggle | Verify Mouse Wheel Up/Down bound in IMC_Adventure |
| Project won't compile | Run Live Coding compile; check .Build.cs dependencies |
| GAS errors on startup | Verify AttributeSet initialized; check UGE_AdventureDefaultAttributes |

---

## Next Phase (After Phase 1 Complete)

Once smoke test passes, next work items:
- [ ] GAS ability implementation (GA_Sprint, GA_Dodge, etc.)
- [ ] Enemy AI (basic Hollowed dummy)
- [ ] Level design (prototype Old Nola Streets)
- [ ] Polish & bug fixes

