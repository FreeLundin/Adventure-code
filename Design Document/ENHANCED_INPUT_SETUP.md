# Enhanced Input System Setup Guide

## Overview
This guide walks through creating Enhanced Input System (EIS) assets for Adventure.

**Assets to create:**
- `IMC_Adventure` — Input Mapping Context (binds actions to keys)
- 9 Input Actions: `IA_Move`, `IA_Look`, `IA_CameraToggle`, `IA_Sprint`, `IA_Dodge`, `IA_AttackLight`, `IA_AttackHeavy`, `IA_Interact`, `IA_Pause`

---

## Method 1: Automated Python Script (Recommended)

### Step 1: Run Creation Script in Editor

1. Open Unreal Editor: `c:\Unreal_Projects\Adventure\Adventure.uproject`
2. In the Editor, go to **Tools → Python Console**
3. Paste and execute:
   ```python
   exec("C:/Unreal_Projects/Adventure/scripts/create_enhanced_input_assets.py")
   ```
4. Check the Output Log — should see "Enhanced Input Asset Creation Complete!"

### Step 2: Add Key/Input Bindings to IMC_Adventure

1. In Content Browser, navigate to `/Game/Input/`
2. Open `IMC_Adventure` (double-click)
3. In the Details panel, click **"+"** under **Mappings** to add a new mapping
4. For each mapping below, create an entry:

| Action Name | Input Key | Modifiers | Value Type |
|-------------|-----------|-----------|-----------|
| IA_Move | Gamepad Left Thumbstick Y | — | Axis2D |
| IA_Move | W | — | Axis2D |
| IA_Move | A | — | Axis2D |
| IA_Move | S | — | Axis2D |
| IA_Move | D | — | Axis2D |
| IA_Look | Gamepad Right Thumbstick | — | Axis2D |
| IA_Look | Mouse X/Y | — | Axis2D |
| IA_CameraToggle | Mouse Wheel Y | — | Digital |
| IA_Sprint | Left Shift | — | Digital |
| IA_Dodge | Space Bar | — | Digital |
| IA_AttackLight | Left Mouse Button | — | Digital |
| IA_AttackHeavy | Right Mouse Button | — | Digital |
| IA_Interact | E | — | Digital |
| IA_Pause | P | — | Digital |

5. **Apply Modifiers** (optional, for advanced input):
   - IA_Move (W/A/S/D): Select mapping → Details → add `FActuationSettings` with threshold
   - IA_Look: Add deadzone for gamepad smoothness

6. **Save** and close IMC_Adventure

---

## Method 2: Manual Creation in Editor (Alternative)

### Step 1: Create Input Actions

1. In Content Browser, right-click in `/Game/Input/`
2. Select **New → Input Action**
3. Name it `IA_Move` and set **Value Type** to **Axis2D**
4. Repeat for each action listed above, setting appropriate value types:
   - **Axis2D for:** IA_Move, IA_Look
   - **Digital for:** IA_CameraToggle, IA_Sprint, IA_Dodge, IA_AttackLight, IA_AttackHeavy, IA_Interact, IA_Pause

### Step 2: Create Input Mapping Context

1. Right-click in `/Game/Input/`
2. Select **New → Input Mapping Context**
3. Name it `IMC_Adventure`
4. Double-click to open
5. Follow Step 2 from Method 1 above to add key bindings

---

## Integration with PlayerController

Once IMC_Adventure is set up, it will be automatically loaded by `APC_AdventureController::BeginPlay()`:

```cpp
void APC_AdventureController::BeginPlay()
{
    Super::BeginPlay();
    
    if (UEnhancedInputSubsystem* Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputSubsystem>())
    {
        Subsystem->AddMappingContext(IMC_Adventure, 0);
    }
}
```

---

## Verification Checklist

- [ ] `/Game/Input/IMC_Adventure` exists and opens without errors
- [ ] `/Game/Input/IA_Move`, `IA_Look`, `IA_CameraToggle`, etc. all exist
- [ ] IMC_Adventure has 14+ mappings configured
- [ ] Each action is linked to appropriate key(s)
- [ ] Editor builds successfully
- [ ] Test in PIE: press W/A/S/D → character moves (once movement is implemented)
- [ ] Mouse wheel press in PIE → camera cycles (once camera toggle is implemented)

---

## Troubleshooting

**Issue:** "Script not found" when running Python script
- **Solution:** Ensure you're using the exact path: `C:/Unreal_Projects/Adventure/scripts/create_enhanced_input_assets.py` (forward slashes)

**Issue:** Input assets not appearing in Content Browser
- **Solution:** Click **View Options** (bottom-right of Content Browser) → Enable **Show Engine Content** and **Show Plugin Content**; refresh view with **F5**

**Issue:** "Default Pawn Class does not implement Enhanced Input"
- **Solution:** Ensure `PC_AdventureController` is set as PlayerControllerClass in `GM_AdventureMode`. See Task 3 for details.

---

## Next Steps

1. ✅ Create Enhanced Input assets (this guide)
2. ⏳ Set DefaultPawn to `ACBP_AdventureCharacter_Mover` in `GM_AdventureMode`
3. ⏳ Test input routing with PIE
4. ⏳ Implement camera toggle based on `IA_CameraToggle` activation
5. ⏳ Implement core movement responding to `IA_Move` and `IA_Sprint`

---

## References

- **UE5 Enhanced Input System docs:** https://docs.unrealengine.com/5.0/en-US/enhanced-input-user-guide/
- **GAS + Enhanced Input integration:** `Source/Adventure/Public/Core/PC_AdventureController.h`
- **Project-wide input config:** `Config/DefaultInput.ini`
