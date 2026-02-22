# Test Level Setup Guide

**Date:** Feb 21, 2026  
**Purpose:** Create minimal playable test level for Phase 1 camera/movement/GAS validation  
**Estimated Time:** 2-3 hours (first time); 1 hour (subsequent updates)  
**Deliverable:** `/Game/Levels/VS_TestLevel.umap`

---

## Level Overview

**Vertical Slice Test Level** — Minimal playable area to validate:
- ✅ Player spawn and possession
- ✅ Camera mode cycling (Top-Down → Third-Person → First-Person)
- ✅ Movement input routing to Mover2 character
- ✅ GAS ability activation via Enhanced Input
- ✅ HUD bindings to AttributeSet (once HUD widget created)

**Size Profile:** 
- ~3000x3000 units (compact, fast iterate)
- Flat ground + raised platform for elevation testing
- Simple geometry to minimize visual noise during debugging

---

## Quick Setup (5 Steps, ~30 minutes)

### Step 1: Create New Level
1. **File → New Level**
2. **Select:** "Blank Level"
3. **Save as:** `/Game/Levels/VS_TestLevel`
4. **Press:** Ctrl+S to save

### Step 2: Place Player Start
1. In **Place Actors** panel (left sidebar), search for "PlayerStart"
2. **Drag PlayerStart object** into the level (or press Shift+Click)
3. **Position:** Set Location to (0, 0, 100) via Details panel
4. **Verify:** Player Controller detects this spawn point at level start

### Step 3: Add Simple Floor
1. **Place Actors → Search "Cube"**
2. **Drag Cube** into level
3. **Scale:** Set Scale to (50, 50, 1) to create a large flat floor
4. **Position:** Set Z to -50 (below player start) so player lands on it
5. **Material:** Right-click → Apply Material → Select any default material (e.g., "M_Wood_Floor_T")

### Step 4: Add Elevation Test Platform
1. **Duplicate the Cube** (Ctrl+D) or place another Cube
2. **Position:** Move 2000 units forward (Y = 2000)
3. **Scale:** Set to (30, 30, 1)
4. **Elevation:** Raise Z to 500 units (testing jump/climb to elevated platform)
5. **Naming:** Rename to "TestPlatform" for clarity

### Step 5: Test in PIE
1. **Play → Play (Editor) — Alt+P**
2. **Verify:**
   - Player character spawns at player start
   - Can move with WASD (character moves if animations/movement implemented)
   - Mouse wheel cycles camera modes
   - HUD displays on screen (once widget added to viewport)
3. **Stop with ESC**

---

## Detailed Setup (Comprehensive, ~2 hours)

**For more polished feel and comprehensive testing, expand Step 3-4 above:**

### Enhanced Level Geometry

#### Building the Test Arena

1. **Create Playable Area:**
   - **Floor:** 100x100 units base platform
   - **Walls:** Add invisible collision walls (cube set to "Collision Only") at edges
   - **Ramps:** 45° angled floor sections (for slope testing)
   - **Gaps:** 1-2 unit gaps between platforms (for jump validation)

2. **Camera Mode Showcase Areas:**

   **Zone A — Top-Down Testing (Center)**
   - Flat open area, 50x50 units
   - Place 4 dummy actors in a circle (to test targeting/camera tracking)
   - Spawn point here for top-down default

   **Zone B — Third-Person Combat (Forward, Y+2000)**
   - Combat arena with elevated pillars
   - Place 3-4 cube markers (for parry/dodge practice)
   - Add one floating platform above (for traversal testing)

   **Zone C — First-Person Investigation (Left Side, X-1500)**
   - Narrow corridor with interactive props
   - Place a door frame (for interaction testing)
   - Add a small ritual diagram decal (visual theme)

3. **Traversal Testing:**
   - **Vault Test:** Low wall (3 units high) between Zone A and Zone B
   - **Mantle Test:** Platform edge at character shoulder height
   - **Climb Test:** Rope/chain prop (vertical mesh) for climbing validation

### Adding Lighting & Sky

1. **Directional Light:**
   - Already in blank level; adjust rotation for dramatic shadows
   - Set Intensity to 1.5 for better visibility of animations

2. **Sky Sphere:**
   - **Place Actors → Sky Sphere**
   - Adds simple sky + atmospheric lighting
   - Optional: adjust sun angle via rotation for time-of-day feel

### Adding Debug Widgets & UI

1. **HUD Overlay:**
   - Once `WB_HUD` created, add to viewport in Level Blueprint or PlayerController BeginPlay
   - See "Create HUD Widget" section for details

2. **Debug Info:**
   - Optional: Add on-screen text via Level Blueprint showing:
     - Current camera mode (Top-Down / Third-Person / First-Person)
     - Player velocity
     - GAS ability status

---

## Checkpoint Validation Checklist

### Spawn & Possess
- [ ] Player controller spawns at PlayerStart location
- [ ] Character is possessed after spawn
- [ ] No placement errors in Editor log

### Input Routing
- [ ] Press W/A/S/D → Character moves (or animation plays if implemented)
- [ ] Press mouse wheel → Camera cycles through modes
- [ ] Logger outputs show "Input received" when keys pressed

### Camera Cycling
- [ ] Scroll wheel up → Switches to next camera mode
- [ ] Scroll wheel down → Switches to previous camera mode
- [ ] All three modes cycle smoothly (Top-Down → Third → First → Top-Down)

### GAS Integration
- [ ] HUD shows Health/Stamina/RitualEnergy bars (once widget added)
- [ ] Press Shift → Prints "Sprint activated" in log (or ability executes)
- [ ] Press E → Prints "Interact activated" in log
- [ ] Ability icons/cooldowns display on HUD (once implemented)

### Level Essentials
- [ ] No floating geometry; player stands on floor
- [ ] Lighting is visible (not completely dark or blown out)
- [ ] Camera doesn't clip through geometry
- [ ] Performance is 60+ FPS in PIE

---

## File Structure After Completion

```
/Game/
├── Levels/
│   ├── VS_TestLevel.umap (THIS FILE — the test level)
│   └── (future levels here)
├── Input/
│   ├── IMC_Adventure.uasset
│   └── IA_*.uasset (from Task 1)
├── Blueprints/
│   └── WB_HUD.uasset (from Task 5)
└── Characters/
    ├── ACBP_AdventureCharacter
    └── ACBP_AdventureCharacter_Mover (DefaultPawn from Task 3)
```

---

## Integration with GameMode

The test level automatically uses `AGM_AdventureMode` if:
1. ✅ Project default GameMode = `AGM_AdventureMode`
2. ✅ DefaultPawnClass = `ACBP_AdventureCharacter_Mover` (set in Task 3)
3. ✅ PlayerControllerClass = `APC_AdventureController`

**To verify:**
- Open VS_TestLevel
- Window → World Settings
- Check "GameMode Override" is set to `AGM_AdventureMode`
- If blank, leave blank (will use project default)

---

## Iteration Workflow

### Testing Movement (Once Movement Implemented)
1. Open VS_TestLevel
2. Adjust DefaultPawnClass or place alternate character instances for A/B testing
3. Iterate on animation selection (ALS vs. Mannequin — Task 2)
4. Tweak speed curves in Character blueprint

### Testing Camera Modes (Once Camera Toggle Implemented)
1. Use Zone A (top-down), Zone B (third-person), Zone C (first-person) areas
2. Verify smooth transitions between modes
3. Check target lock (if camera focus on enemy needed)

### Testing Combat (Once Ability Implementation)
1. Place enemy dummies in Zone B
2. Activate abilities via input keys
3. Verify GAS ability activation, cooldown display

---

## Troubleshooting

**Issue:** Player falls through floor
- **Solution:** Verify floor mesh collision is set to "BlockAll" (right-click mesh → Collision)

**Issue:** Player spawns at level origin, not at PlayerStart
- **Solution:** Ensure PlayerStart has "Initial Location" set in Details, or set Player Controller default spawn location

**Issue:** Camera is inside character head
- **Solution:** Adjust camera boom distance in `ACBP_AdventureCharacter` blueprint (CameraBoom ThirdPersonDistance)

**Issue:** Input not working
- **Solution:** Verify Enhanced Input Mapping Context (IMC_Adventure) is applied in `APC_AdventureController::BeginPlay()`

---

## Next Steps

1. ✅ Create test level (this guide)
2. ⏳ Create HUD widget (Task 5) — then bind to this level's viewport
3. ⏳ Implement camera toggle (Task 6) — test cycling in this level
4. ⏳ Implement core movement (Task 7) — iterate movement feel in this level
5. (Future) Add enemies and expand to full level once gameplay loop validated

---

## References

- **UE5 Lighting:** https://docs.unrealengine.com/5.0/en-US/lighting-basics-in-unreal-engine/
- **Level Blueprint:** Right-click in level → Open Level Blueprint to add custom logic
- **Play Settings:** Edit → Project Settings → Engine → General Settings → for default level/gamemode
- **Character Setup:** See `CBP_AdventureCharacter_Mover.h` for camera boom and pawn class validation

