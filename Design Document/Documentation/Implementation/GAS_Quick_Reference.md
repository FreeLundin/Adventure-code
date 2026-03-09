# GAS Integration Quick Reference (Adventure)

## Quick Setup Checklist

- C++ foundation compiled (Live Coding build succeeds)
- BP derived from `CBP_AdventureCharacter` or `CBP_AdventureCharacter_Mover`
- Enhanced Input assets created and mapped (IMC_Adventure, IA_*)

## Core Classes

- `AAdventurePlayerState` (PlayerState-owned ASC)
- `UAdventureAttributeSet` (Health, Stamina, RitualEnergy)
- `UGA_AdventureSprint`, `UGA_AdventureDodge`, `UGA_AdventureTraversal`, `UGA_AdventureInteract`
- `UGE_AdventureDefaultAttributes` (instant default attributes GE)

## Blueprint Integration

1. Create a BP from the desired character class.
2. Confirm DefaultAbilities and DefaultAttributesEffect in the GAS category.
3. Add the BP to your GameMode as Default Pawn.

## Common Blueprint Nodes

- Get Ability System Component → Try Activate Abilities by Tag
- Get Ability System Component → Get Gameplay Attribute Value
- Apply Gameplay Effect to Self

## Tags

- `Ability.Sprint`
- `Ability.Dodge`
- `Ability.Traversal`
- `Ability.Interact`

## Typical Test Flow

1. PIE with the BP pawn
2. Verify attributes initialize (Health/Stamina/RitualEnergy at 100)
3. Trigger inputs to activate Sprint/Dodge/Traversal/Interact
4. Bind HUD to UAdventureAttributeSet values

## Next Targets

- Implement GA_LightAttack/GA_HeavyAttack
- Create GE_RitualEnergyGain effects for +3/+8/+10/+12/+20
- Add cost/cooldown effects for movement abilities
