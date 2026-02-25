# Phase 1 Quick Action Plan
**Execute in order | Estimated Total Time: 2-3 hours**

---

## TODAY'S EXECUTION PLAN

### **PHASE 1A: INPUT SYSTEM (30 min)**

**[Status: In Progress]** Current step

```
□ STEP 1: Run Enhanced Input Script (5 min)
  Location: Tools → Python Console
  Command:
  import runpy
  runpy.run_path("C:/Unreal_Projects/Adventure/scripts/create_enhanced_input_assets.py")
  Expected: 11 assets created in Content/Input/

□ STEP 2: Bind Input Keys (20 min)
  Location: Content/Input/IMC_Adventure (double-click)
  Guide: Design Document/INPUT_KEY_BINDING_REFERENCE.md
  
  Quick bindings needed:
  · IA_Move: W, A, S, D
  · IA_Look: Mouse X, Mouse Y
  · IA_Sprint: Left Shift
  · IA_Dodge: Spacebar
  · IA_LightAttack: Left Mouse
  · IA_HeavyAttack: Right Mouse
  · IA_Interact: E
  · IA_Parry: Q
  · IA_CameraToggle: Mouse Wheel Up/Down
  
  Save when done (Ctrl+S)

□ STEP 3: Verify Input System (5 min)
  Check: Source/Adventure/Private/Core/PC_AdventureController.cpp
  Verify: SpawnHUD() includes EnhancedInput setup
```

✅ **Phase 1A Deliverable:** Input system responds to keys

---

### **PHASE 1B: TEST LEVEL (20 min)**

```
□ STEP 4: Create Test Level (15 min)
  File → New Level → Default Blank
  Save As: Content/Levels/VS_TestLevel
  Add: Basic floor plane (optional walls)
  Add: Player Start at center

□ STEP 5: Set Default Maps (5 min)
  Edit → Project Settings → Maps
  Set:
  · Editor Startup Map: /Game/Levels/VS_TestLevel
  · Game Default Map: /Game/Levels/VS_TestLevel
  Save
```

✅ **Phase 1B Deliverable:** Test level ready with PlayerStart

---

### **PHASE 1C: HUD WIDGET (30 min)**

```
□ STEP 6: Create HUD Blueprint (20 min)
  Right-click Content/UI → Blueprint Class → WB_HUD
  Name: BP_HUD_Main
  
  Designer Layout:
  · HealthBar (ProgressBar, red)
  · StaminaBar (ProgressBar, green)
  · RitualEnergyBar (ProgressBar, purple)
  
  Compile & Save

□ STEP 7: Verify HUD Wiring (10 min)
  Check: UWB_HUD C++ code includes all delegate bindings
  Expected: InitializeHUD() auto-discovers AttributeSet
```

✅ **Phase 1C Deliverable:** HUD displays on screen during play

---

### **PHASE 1D: ANIMATIONS (Decision Point)**

```
□ STEP 8: Choose Animation Path
  Option A: Mannequin (7-9 hours, recommended for Phase 1)
  Option B: ALS v4 (15-20 hours, more comprehensive)
  Option C: None (skip, test movement logic only)
  
  Decision: _______________
```

✅ **Phase 1D Deliverable:** Animation retarget plan (if chosen)

---

### **PHASE 1E: SMOKE TEST (15 min)**

```
□ STEP 9: Play in Editor
  Alt+P (or Play button)
  
  Tests:
  ✓ W/A/S/D → character moves
  ✓ Shift → sprint (if animations added)
  ✓ Mouse Wheel → camera cycles
  ✓ HUD bars visible
  ✓ No crashes or errors
  
  Exit: Esc

□ STEP 10: Validation
  Run script in Python Console:
  exec("C:/Unreal_Projects/Adventure/scripts/validate_phase1.py")
  
  Expected: All checks passing
```

✅ **Phase 1E Deliverable:** Smoke test passing

---

### **PHASE 1F: COMPLETE & COMMIT (5 min)**

```
□ STEP 11: Final Git Commit
  Terminal:
  cd c:\Unreal_Projects\Adventure
  git add -A
  git commit -m "feat(phase1): complete setup - input, HUD, test level, smoke test passing"
  
□ STEP 12: Tag Milestone
  git tag -a "phase1-complete" -m "Phase 1 setup complete and smoke test passing"
  git push (if applicable)
```

✅ **Phase 1F Deliverable:** Git history recorded

---

## Time Breakdown

| Phase | Task | Time | Status |
|-------|------|------|--------|
| 1A | Input System | 30 min | ⏳ IN PROGRESS |
| 1B | Test Level | 20 min | ⏳ PENDING |
| 1C | HUD Setup | 30 min | ⏳ PENDING |
| 1D | Animations | 0-20 min | ⏳ DECISION |
| 1E | Smoke Test | 15 min | ⏳ PENDING |
| 1F | Commit & Tag | 5 min | ⏳ PENDING |
| | **TOTAL** | **95-115 min** | |

---

## During Execution

### If Stuck:
1. **Input not working?** → Check IMC_Adventure key bindings
2. **Level won't load?** → Verify PlayerStart is placed
3. **HUD not showing?** → Check WB_HUD initialization in PC
4. **Character invisible?** → Check character Blueprint assignment
5. **GAS errors?** → See LIVE_CODING_FIX.md

### Validation Scripts:
- `validate_phase1.py` ← Run anytime to check progress
- `create_test_level.py` ← Can auto-create test level
- `create_enhanced_input_assets.py` ← Already executed

---

## After Phase 1 Complete

Next work items:
- [ ] GAS Ability Implementation (GA_Sprint, GA_Dodge, etc.)
- [ ] Enemy AI Framework
- [ ] Level Design Iteration
- [ ] Animation Polish

---

**START WITH STEP 1 (Enhanced Input Script)**

Once that's complete and verified, proceed to STEP 2 (Bind Keys).

Ready? 👍
