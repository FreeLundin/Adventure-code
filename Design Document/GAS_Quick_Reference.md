# GAS Integration Quick Reference

## Quick Setup Checklist

✅ **C++ Classes Created:**
- `AGASCharacter` - Base character with GAS support (Parent for blueprints)
- `UGASAttributeSet` - Contains RitualEnergy, Health, and Stamina attributes
- `APhase1PlayerController` - Controller with console commands for testing

✅ **Attributes Available:**
- `RitualEnergy` / `MaxRitualEnergy` - Primary resource for abilities
- `Health` / `MaxHealth` - Character health
- `Stamina` / `MaxStamina` - For movement abilities (sprint, dodge)

## Blueprint Integration (CBP_SandboxCharacter_Mover)

### Step 1: Reparent
File → Reparent Blueprint → Select `GASCharacter`

### Step 2: Set Default Stats (Optional)
Create `GE_DefaultStats` (GameplayEffect) → Add to Default Effects array

### Step 3: Add Abilities (Optional)
Create abilities → Add to Default Abilities array

## Console Commands (Testing)

```
AddRitualEnergy 50       # Add 50 ritual energy
SetRitualEnergy 100      # Set ritual energy (adds value)
showdebug abilitysystem  # Show ASC debug info
```

## Blueprint Nodes (Common Operations)

### Get Attribute Value
```
Get Ability System Component → Get Gameplay Attribute Value
  └─ Attribute: RitualEnergy (or Health, Stamina)
```

### Modify Attribute
```
Get Ability System Component → Apply Mod to Attribute
  ├─ Attribute: RitualEnergy
  ├─ Mod Op: Additive
  └─ Magnitude: 10.0
```

### Check if Has Ability
```
Get Ability System Component → Get Abilities
```

## Creating Gameplay Effects

### Instant Effect (e.g., Damage, Heal)
- Duration Policy: **Instant**
- Modifier: `Health` → `Add` → `-20.0`

### Duration Effect (e.g., Buff)
- Duration Policy: **Has Duration**
- Duration Magnitude: `5.0` seconds
- Modifier: `MaxHealth` → `Multiply` → `1.2`

### Periodic Effect (e.g., DoT, Regen)
- Duration Policy: **Infinite** (or Has Duration)
- Period: `1.0` seconds
- Modifier: `Health` → `Add` → `5.0` per tick

## Creating Gameplay Abilities

### Basic Ability Template
1. Right-click → Blueprint Class → GameplayAbility
2. Name it (e.g., `GA_Jump`, `GA_Attack`)
3. Configure:
   - Ability Tags (e.g., `ability.jump`)
   - Cost GE Class (e.g., `GE_JumpCost`)
   - Cooldown GE Class (e.g., `GE_JumpCooldown`)
4. Implement Activate Ability logic

### Ability with Cost
```
On Activate:
  ├─ Commit Ability (checks cost/cooldown)
  ├─ Play Montage or Execute Logic
  └─ End Ability
```

## GAS Architecture Overview

```
Character (AGASCharacter)
  ├─ Ability System Component
  │    ├─ Granted Abilities (GA_*)
  │    ├─ Active Effects (GE_*)
  │    └─ Gameplay Tags
  └─ Attribute Set (UGASAttributeSet)
       ├─ RitualEnergy / MaxRitualEnergy
       ├─ Health / MaxHealth
       └─ Stamina / MaxStamina
```

## Phase 1 Implementation Order

1. ✅ Reparent CBP to GASCharacter
2. ✅ Create GE_DefaultStats
3. Create GE_JumpCost → Link to GA_Jump
4. Create GE_SprintCost + GE_SprintModify → GA_Sprint
5. Create GE_Regen → GA_PassiveRegen
6. Bind abilities to input
7. Connect attributes to HUD

## Common Issues & Solutions

**Attributes not showing:**
→ Verify blueprint reparented to GASCharacter
→ Check Gameplay Abilities plugin is enabled

**Console commands not working:**
→ Ensure controller is Phase1PlayerController
→ Verify character has Ability System Component

**Abilities not activating:**
→ Check ability is granted (Default Abilities array)
→ Verify input binding
→ Check cost requirements (enough stamina/energy?)

## Documentation

- Full Guide: `Documentation/How_to_Add_GAS_to_Mover.md`
- GAS Backlog: `Documentation/GAS_Backlog.md`
- Phase 1 Plan: `Documentation/Phase1_Burndown.md`

## Example: Adding Sprint Ability

1. Create `GE_SprintCost`: Duration=Infinite, Period=0.5s, Stamina Add -5
2. Create `GA_Sprint`: On Activate → Apply GE_SprintCost, Increase walk speed
3. On Input Release → Remove effect, End Ability
4. Add GA_Sprint to Default Abilities in character
5. Bind Shift key to Try Activate Ability by Tag: `ability.sprint`

## Testing Workflow

1. Place character in test level
2. PIE (Alt+P)
3. Console: `showdebug abilitysystem`
4. Console: `AddRitualEnergy 50` (verify UI updates)
5. Test abilities (jump, sprint, etc.)
6. Verify attribute changes in debug overlay
