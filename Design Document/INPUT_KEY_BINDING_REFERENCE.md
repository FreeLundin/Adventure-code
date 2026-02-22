# Phase 1A.2: Input Key Binding Reference
**Quick Binding Guide for IMC_Adventure**

---

## Quick Reference Table

**Copy-paste this layout into IMC_Adventure:**

```
╔═══════════════════════════════════════════════════════════════════════════╗
║ Action Name      │ Key(s)           │ Value Type │ Value             │    ║
╠═══════════════════════════════════════════════════════════════════════════╣
║ IA_Move          │ W / A / S / D    │ Value      │ 1D (WASD array)   │ ✓  ║
║ IA_Look          │ Mouse Movement   │ Value      │ 2D (X, Y)         │ ✓  ║
║ IA_Sprint        │ Left Shift       │ Digital    │ 1.0               │ ✓  ║
║ IA_Dodge         │ Spacebar         │ Digital    │ 1.0               │ ✓  ║
║ IA_LightAttack   │ Left Mouse       │ Digital    │ 1.0               │ ✓  ║
║ IA_HeavyAttack   │ Right Mouse      │ Digital    │ 1.0               │ ✓  ║
║ IA_Interact      │ E Key            │ Digital    │ 1.0               │ ✓  ║
║ IA_Parry         │ Q Key            │ Digital    │ 1.0               │ ✓  ║
║ IA_CameraToggle  │ Mouse Wheel ↑↓   │ Digital    │ +1.0 / -1.0       │ ✓  ║
╚═══════════════════════════════════════════════════════════════════════════╝
```

---

## Step-by-Step Binding Instructions

### **OPEN IMC_Adventure**

```
1. Editor Window → Content Browser (bottom)
2. Navigate to: Content/Input/
3. Double-click: IMC_Adventure
   (Opens Input Mapping Context editor)
```

---

### **BIND IA_Move (WASD)**

```
1. In IMC_Adventure Details Panel on right:
   Click: [+] Add Mapping

2. First mapping:
   - Action: IA_Move
   - Key: W Key
   - Value Type: Value (1D Axis)

3. Repeat for A, S, D keys:
   [+] Add Mapping → IA_Move → A Key
   [+] Add Mapping → IA_Move → S Key
   [+] Add Mapping → IA_Move → D Key

4. Verify in Details:
   ✓ Should show 4 entries (W, A, S, D)
```

---

### **BIND IA_Look (Mouse)**

```
1. [+] Add Mapping → IA_Look → Mouse X
   Value Type: Value (1D Axis)

2. [+] Add Mapping → IA_Look → Mouse Y
   Value Type: Value (1D Axis)

3. Verify:
   ✓ Should show 2 entries (Mouse X, Mouse Y)
```

---

### **BIND IA_Sprint (Shift)**

```
1. [+] Add Mapping → IA_Sprint → Left Shift
   Value Type: Digital (1.0)

2. Save
```

---

### **BIND IA_Dodge (Spacebar)**

```
1. [+] Add Mapping → IA_Dodge → Spacebar
   Value Type: Digital (1.0)

2. Save
```

---

### **BIND IA_LightAttack (Left Mouse)**

```
1. [+] Add Mapping → IA_LightAttack → Left Mouse Button
   Value Type: Digital (1.0)

2. Save
```

---

### **BIND IA_HeavyAttack (Right Mouse)**

```
1. [+] Add Mapping → IA_HeavyAttack → Right Mouse Button
   Value Type: Digital (1.0)

2. Save
```

---

### **BIND IA_Interact (E)**

```
1. [+] Add Mapping → IA_Interact → E Key
   Value Type: Digital (1.0)

2. Save
```

---

### **BIND IA_Parry (Q)**

```
1. [+] Add Mapping → IA_Parry → Q Key
   Value Type: Digital (1.0)

2. Save
```

---

### **BIND IA_CameraToggle (Mouse Wheel)**

```
1. [+] Add Mapping → IA_CameraToggle → Mouse Wheel Up
   Value Type: Digital
   Set Modifiers/Properties:
   - Value: 1.0

2. [+] Add Mapping → IA_CameraToggle → Mouse Wheel Down
   Value Type: Digital
   Set Modifiers/Properties:
   - Value: -1.0

3. Save
```

---

## Final Verification

After binding all keys:

```
1. Save IMC_Adventure (Ctrl+S)

2. Close the editor

3. In Content Browser verify:
   ✓ IMC_Adventure file shows as modified (white dot)
   ✓ Close and re-open → confirms saved

4. Next: Create Test Level (Phase 1B)
```

---

## Troubleshooting

| Problem | Solution |
|---------|----------|
| Key not appearing in dropdown | Try alternative name (e.g., "Left Shift" vs "Left Shift Key") |
| Mapping not saving | Ensure .umap file has write permissions; check Project Settings |
| Duplicate or conflicting bindings | Each action should have unique key + modifier combo |
| Spacebar not recognized | Try "Space Key" or "Spacebar" (case-sensitive) |

---

## Next Phase

Once all keys are bound and saved:
→ **Phase 1B: Create Test Level** (20 min)

