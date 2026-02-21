# How to Add GAS to CBP_SandboxCharacter_Mover

This guide explains how to integrate the Gameplay Ability System (GAS) into your CBP_SandboxCharacter_Mover blueprint using the newly created C++ classes.

## Overview

The Gameplay Ability System provides a flexible framework for implementing abilities, attributes (like Health, Stamina, RitualEnergy), and gameplay effects in your character.

## Prerequisites

1. Ensure the following plugins are enabled in your project:
   - GameplayAbilities
   - GameplayTasks
   - GameplayTags

2. The C++ classes have been created:
   - `AGASCharacter` - Base character class with GAS support
   - `UGASAttributeSet` - Attribute set containing RitualEnergy, Health, and Stamina

## Step-by-Step Integration

### Step 1: Reparent the Blueprint

1. Open `CBP_SandboxCharacter_Mover` in the Blueprint Editor
2. Go to **File → Reparent Blueprint**
3. Select `GASCharacter` as the new parent class
4. Save the blueprint

After reparenting, your blueprint will automatically have:
- An Ability System Component
- A GAS Attribute Set with RitualEnergy, Health, and Stamina attributes

### Step 2: Configure Default Attributes (Optional)

To set initial attribute values, you'll need to create a Gameplay Effect:

1. In the Content Browser, right-click → **Blueprint Class**
2. Search for and select **GameplayEffect**
3. Name it `GE_DefaultStats`
4. Open the effect and configure:
   - **Duration Policy**: Instant
   - Add **Modifiers** for each attribute:
     - `RitualEnergy` → Set to 0.0 (or desired starting value)
     - `MaxRitualEnergy` → Set to 100.0
     - `Health` → Set to 100.0
     - `MaxHealth` → Set to 100.0
     - `Stamina` → Set to 100.0
     - `MaxStamina` → Set to 100.0

5. Back in `CBP_SandboxCharacter_Mover`:
   - Find the **Default Effects** array in the Class Defaults
   - Add `GE_DefaultStats` to the array

### Step 3: Verify GAS Integration

1. Place your character in a test level
2. Start Play-in-Editor (PIE)
3. Open the console (`) and type: `showdebug abilitysystem`
4. You should see the Ability System Component information and current attribute values

### Step 4: Access Attributes from Blueprint

You can now access and modify attributes in Blueprint:

**Get Attribute Value:**
```
Get Ability System Component → Get Gameplay Attribute Value
```

**Modify Attribute:**
```
Get Ability System Component → Apply Mod to Attribute
```

Example: Adding RitualEnergy
- Attribute: `RitualEnergy` from `UGASAttributeSet`
- Mod Op: `Additive`
- Magnitude: `10.0` (or desired amount)

### Step 5: Bind HUD to Attributes (Optional)

If you have a HUD widget (WB_HUD):

1. In your HUD widget, add functions to get attribute values:
   - Get Player Pawn → Cast to GASCharacter
   - Get Attribute Set
   - Get the desired attribute value (RitualEnergy, Health, etc.)

2. Bind progress bars or text to these attribute values

## Using the Phase1PlayerController Functions

The `APhase1PlayerController` class already has helper functions that work with GAS:

- `AddRitualEnergy <amount>` - Console command to add ritual energy
- `SetRitualEnergy <value>` - Console command to set ritual energy (actually adds)

These commands will work automatically once your character has the GAS setup.

## Creating Abilities

To create a gameplay ability:

1. Right-click in Content Browser → **Blueprint Class**
2. Search for **GameplayAbility**
3. Name it (e.g., `GA_Jump`, `GA_Sprint`)
4. Configure the ability:
   - Set Input ID if binding to input
   - Configure ability tags
   - Set cost Gameplay Effect if needed
5. Add the ability to the **Default Abilities** array in your character

## Common Gameplay Effects

### Cost Effect (e.g., Jump Cost)
- Duration Policy: **Instant**
- Modifier: `Stamina` → `Add` → `-10.0`

### Continuous Cost (e.g., Sprint)
- Duration Policy: **Has Duration** or **Infinite**
- Period: `0.5` seconds
- Modifier: `Stamina` → `Add` → `-5.0` per period

### Regeneration Effect
- Duration Policy: **Infinite**
- Period: `1.0` seconds
- Modifier: `Stamina` → `Add` → `5.0` per period

## Troubleshooting

### Attributes not showing up
- Verify the blueprint is properly reparented to GASCharacter
- Check that the Ability System Component is present in the Components panel
- Make sure Gameplay Abilities plugin is enabled

### Attribute changes not replicating
- Ensure your character's replication settings are correct
- The Ability System Component should have "Replicate" enabled
- Check network settings in PIE (test with multiple clients)

### Console commands not working
- Verify the controller is `APhase1PlayerController`
- Make sure the pawn has an Ability System Component
- Check that the attribute set exists on the character

## Next Steps

After GAS is integrated:

1. Create abilities for Jump, Sprint, Dodge (as per GAS_Backlog.md)
2. Create Gameplay Effects for costs and buffs
3. Implement ability input bindings
4. Connect attributes to your HUD
5. Test the full gameplay loop with RitualEnergy gains and costs

## Reference

- Documentation: `Documentation/GAS_Backlog.md`
- Implementation Plan: `Design Document/Adventure_Implementation_Plan.md`
- Phase 1 Burndown: `Documentation/Phase1_Burndown.md`

## Support

If you encounter issues:
1. Check the Output Log for GAS initialization messages
2. Use `showdebug abilitysystem` to inspect the ASC state
3. Verify that all required plugins are enabled
4. Ensure the C++ code has been compiled successfully
