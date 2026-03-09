# HUD Widget Setup Guide

**Date:** Feb 21, 2026  
**Purpose:** Create and configure main gameplay HUD widget (WB_HUD) with attribute bindings  
**Estimated Time:** 2-3 hours (UMG design + Blueprint setup)  
**Deliverable:** `/Game/UI/WB_HUD.uasset` (Blueprint widget class)

---

## Overview

**WB_HUD** is the main gameplay interface displaying:
- ✅ **Health Bar** — Player current/max health with red fill
- ✅ **Stamina Bar** — Player stamina with green fill
- ✅ **Ritual Energy Bar** — Player ritual energy with purple fill
- ✅ **Attribute Text** — Current/max values for each bar (e.g., "100.0 / 100.0")
- ⏳ (Future) Ability cooldown indicators
- ⏳ (Future) Combat feedback (crit, dodge animations)
- ⏳ (Future) Artifact status icons

**Technical Foundation:**
- C++ base class `UWB_HUD` provides attribute binding and update logic
- Blueprint child class `WB_HUD_BP` provides UX/styling
- Widget auto-initializes on player controller `BeginPlay()`
- Real-time updates via GAS attribute change delegates

---

## Quick Setup (30 minutes)

### Step 1: Create Widget Blueprint

1. In Content Browser, navigate to `/Game/UI/` (create if doesn't exist)
2. **Right-click → Create → Widget Blueprint**
3. Select **UWB_HUD** as the parent class
4. Name it **WB_HUD** (or **WB_HUD_BP**)
5. **Open** the new widget

### Step 2: Add Visual Layout

1. **Root Canvas Panel:**
   - Drag a **Canvas Panel** into the viewport (if not already there)
   - Set Position to (0, 0), Size to (1920, 1080)
   - Anchors: Top-Left (stretch fill)

2. **Health Bar Container:** (Top-left corner)
   - Add a **Vertical Box** widget
   - Position: (20, 20), Size: (200, 50)
   - Add child: **Text Block** named `HealthLabel` (text="HEALTH")
   - Add child: **ProgressBar** named `HealthBar` (fill color red)
   - Add child: **Text Block** named `HealthText` (text="100 / 100")

3. **Stamina Bar Container:** (Below Health)
   - Add a **Vertical Box**
   - Position: (20, 80), Size: (200, 50)
   - Add child: **Text Block** named `StaminaLabel` (text="STAMINA")
   - Add child: **ProgressBar** named `StaminaBar` (fill color green)
   - Add child: **Text Block** named `StaminaText` (text="100 / 100")

4. **Ritual Energy Bar Container:** (Below Stamina)
   - Add a **Vertical Box**
   - Position: (20, 140), Size: (200, 50)
   - Add child: **Text Block** named `RitualEnergyLabel` (text="RITUAL ENERGY")
   - Add child: **ProgressBar** named `RitualEnergyBar` (fill color purple)
   - Add child: **Text Block** named `RitualEnergyText` (text="100 / 100")

### Step 3: Configure Widget Bindings

In the Details panel for each Progress Bar widget:
- Set **Fill Color And Opacity** to your chosen color (per Step 2)
- Set **Fill Style** to "Fill from Left" or "Fill from Center"
- Set **Percent** to 0.5 (will be overridden by C++ code)

### Step 4: Set HUD Class in PlayerController

1. Open Unreal Editor, go to Project Settings
2. Navigate to **Engine → Game → PC_AdventureController Defaults**
3. Set **HUD Widget Class** to `WB_HUD_C` (or **WB_HUD_BP_C**)
4. Save and close settings

---

## Detailed Setup (Comprehensive UX, ~2 hours)

**For a polished HUD with animations and visual feedback:**

### Advanced Layout with Anchor System

1. **Create Responsive Layout Grid:**
   - Add a **Horizontal Box** at root (for left/center/right sections)
   - **Left Section:** Attributes (health/stamina/ritual) — 400px wide
   - **Center Section:** Ability icons (future use) — flexible
   - **Right Section:** Map/objectives (future) — 300px wide

2. **Health Bar with Visual Gradient:**
   - Use a **Canvas Panel** for layering
   - Add background image (dark bar)
   - Add foreground progress bar (red gradient)
   - Add **Border** widget for frame decoration
   - Add text overlay for numeric display

3. **Stamina Bar with Drain Animation:**
   - Progress bar with green fill
   - Add a **Slider** widget (disabled) to show depletion rate
   - Use opacity fade for low-stamina warning

4. **Ritual Energy Bar with Charge Effect:**
   - Purple bar with glow effect
   - Add **Image** widget on top for particle-like effect
   - Use **Animate** blueprint function for charge effect when energy changes rapidly

### Styling & Theme

1. **Colors:**
   - Health Red: `#FF0000`
   - Stamina Green: `#00FF00`
   - Ritual Purple: `#8000FF`
   - Background Dark: `#1A1A1A` with 0.7 opacity
   - Text White: `#FFFFFF`

2. **Fonts:**
   - Labels: Bold Sans (20pt)
   - Numbers: Monospace (18pt)

3. **Animations (UMG Animation Blueprint):**
   - Bar fill transition (0.5 sec ease-out)
   - Low health pulse (red flashing)
   - Ritual energy charge glow

### Low Health Warning System

1. Add a **Canvas Panel** (named `LowHealthWarningPanel`)
2. Add an **Image** widget with red tint (opacity 0.2)
3. In C++ `UpdateHealthBar()`, trigger animation when health < 30%:
   ```
   if (FillPercent < 0.3) 
       PlayAnimation(LowHealthWarningPulse)
   ```

---

## C++ Integration

### Code the Widget Already Handles

The C++ class `UWB_HUD` provides:
- ✅ Auto-discovery of player's AttributeSet via PlayerState ASC
- ✅ Subscription to attribute change delegates (OnRep callbacks)
- ✅ Bar percentage calculation and color logic
- ✅ Text formatting (current/max display)
- ✅ Low health threshold warning

### Blueprint Construction Script (Optional)

In the Blueprint `WB_HUD_BP`, you can add to **Construct** event:
```
Event Construct
  → Print String "HUD Widget Constructed"
  → [This will be called before InitializeHUD runs]
```

---

## Verification Checklist

### Widget Creation
- [ ] `/Game/UI/WB_HUD.uasset` (or WB_HUD_BP.uasset) exists
- [ ] Opens without errors in Blueprint editor
- [ ] All progress bars visible in designer
- [ ] Text blocks show placeholder text

### Inspector Setup
- [ ] Progress bar widgets have **meta = (BindWidget)** in C++ header
  - (Note: This is auto-generated; verify no compiler errors)
- [ ] HealthBar, HealthText, StaminaBar, StaminaText, RitualEnergyBar, RitualEnergyText are all bound
- [ ] Colors set correctly in Blueprint picker

### Runtime Validation (After Compiling)
1. Open VS_TestLevel (from Task 4)
2. Set PC_AdventureController → HUDWidgetClass = WB_HUD_BP_C
3. **Play (PIE)**
4. Verify:
   - [ ] HUD appears on screen in top-left corner
   - [ ] All three bars visible with text displays
   - [ ] No errors in Output Log about "BindWidget" or missing widget components
   - [ ] Bars show initial values (should be 100 / 100 from default GE)

### Attribute Binding Test (Advanced)
1. In Blueprint editor, add Debug Print to test:
   - **Double-click WB_HUD_BP**, go to **Event Graph**
   - Add custom event `TestAttributeUpdate`
   - Event: Print String "Health Changed!"
   - Call this event from C++ during `OnHealthChanged()`
2. **Debug in PIE:**
   - Spawn player
   - If print fires, bindings are working
   - Verify bar percentage changes

---

## File Structure After Setup

```
/Game/
├── UI/
│   ├── WB_HUD.uasset (C++ class created in Task 5)
│   └── WB_HUD_BP.uasset (THIS FILE — Blueprint child)
└── Levels/
    └── VS_TestLevel.umap
```

---

## Common Issues & Fixes

**Issue:** Widget appears but bars don't update
- **Cause:** AttributeSet not found or delegate binding failed
- **Fix:** Check Output Log for "No AttributeSet found" message; verify PlayerState has ASC initialized

**Issue:** Widget doesn't appear at all
- **Cause:** HUDWidgetClass not set in ProjectSettings
- **Fix:** In Project Settings, search "PC_AdventureController", set HUD Widget Class to WB_HUD_BP_C

**Issue:** Text shows "0 / 0" or wrong values
- **Cause:** Default attributes effect not applied
- **Fix:** Verify `UGE_AdventureDefaultAttributes` is created and character initializes GAS on spawn

**Issue:** Widget overlaps/covers important screen area
- **Cause:** Anchor/position not set correctly
- **Fix:** In widget designer, adjust **Anchors** and **Position** for widget components

**Issue:** Compilation error about "BindWidget"
- **Cause:** Progress bar name doesn't match C++ property name
- **Fix:** In Blueprint widget hierarchy, ensure widget names exactly match:
   - **HealthBar** (not "ProgressBar_Health" or other variants)
   - **StaminaBar**
   - **RitualEnergyBar**
   - (Same for Text blocks: **HealthText**, **StaminaText**, **RitualEnergyText**)

---

## Integration with Level

Once WB_HUD is set up:
1. Open VS_TestLevel (from Task 4)
2. **Window → World Settings** → Verify GameMode = AGM_AdventureMode
3. Play (PIE) → Character spawns → HUD appears automatically
4. Moving/combat will update bars in real-time (once movement/combat implemented)

---

## Attribute Update Flow (Reference)

```
Player Input (e.g., Light Attack)
   ↓
GAS Ability Executes (GA_LightAttack)
   ↓
GameplayEffect Applied (Damage to enemy; enemy loses health)
   ↓
EnemyAttributeSet.Health changed
   ↓
OnRep_Health callback fires (if replicated)
   ↓
ASC broadcasts attribute change delegate
   ↓
UWB_HUD::OnHealthChanged() receives callback
   ↓
UpdateHealthBar() updates bar percentage & text
   ↓
UMG **Widget refreshes on screen**
```

---

## Next Steps

1. ✅ Create HUD Widget (this guide)
2. ⏳ Implement camera toggle (Task 6) — Display current camera mode on HUD
3. ⏳ Implement core movement (Task 7) — Movement should deplete stamina
4. (Future) Add ability cooldown UI
5. (Future) Add combat feedback animations (crit numbers, damage pops)

---

## References

- **UMG Designer Guide:** https://docs.unrealengine.com/5.0/en-US/umg-user-interface-design-guide-in-unreal-engine/
- **Progress Bar Widget:** https://docs.unrealengine.com/5.0/en-US/api/Runtime/UMG/Components/UProgressBar/
- **GAS Attribute Delegates:** https://docs.unrealengine.com/5.0/en-US/gameplay-ability-system-in-unreal-engine/
- **C++ WB_HUD Class:** `Source/Adventure/Public/UI/WB_HUD.h`
- **Integration Point:** `APC_AdventureController::SpawnHUD()`

