# How to Add GAS to CBP_AdventureCharacter_Mover

This guide explains how to use the Adventure GAS foundation with the Mover2 character variant.

## Overview

Adventure uses a PlayerState-owned Ability System Component (ASC). Both character variants (CMC and Mover2) initialize GAS on possession and replication.

## Prerequisites

1. Ensure plugins are enabled:
   - GameplayAbilities
   - GameplayTasks
   - GameplayTags

2. C++ classes exist:
   - `AAdventurePlayerState` (ASC owner)
   - `UAdventureAttributeSet` (Health, Stamina, RitualEnergy)
   - `UGE_AdventureDefaultAttributes` (default attribute values)

## Step-by-Step Integration

### Step 1: Create the Blueprint

1. Create a BP derived from `CBP_AdventureCharacter_Mover`.
2. Save it (for example: `BP_AdventureCharacter_Mover`).

### Step 2: Verify GAS Defaults

In Class Defaults (GAS category), confirm:
- DefaultAbilities is populated (Sprint/Dodge/Traversal/Interact stubs).
- DefaultAttributesEffect is set to `UGE_AdventureDefaultAttributes`.

### Step 3: Set GameMode Defaults

- Set Default Pawn Class to your new mover BP.
- Ensure PlayerStateClass is `AAdventurePlayerState` (already set in C++ GameMode).

### Step 4: Create Enhanced Input Assets

- Create `IMC_Adventure` and IA_* actions.
- Bind Move/Look/Jump/Sprint/Crouch if desired.

### Step 5: Test GAS Initialization

1. PIE the level.
2. Use `showdebug abilitysystem` to confirm ASC and attributes.
3. Verify Health/Stamina/RitualEnergy initialize to 100.

## HUD Binding

Bind your HUD widget to `UAdventureAttributeSet` values for Health/Stamina/RitualEnergy.

## Next Steps

- Implement GA_LightAttack/GA_HeavyAttack and RitualEnergy gain effects.
- Add GE cost/cooldown effects for Sprint/Dodge/Traversal.
- Wire traversal abilities to gameplay events or montages.

## Reference

- `Design Document/GAS_Backlog.md`
- `Design Document/GAS_Quick_Reference.md`
- `Design Document/Adventure_Implementation_Plan.md`
