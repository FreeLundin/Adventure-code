# Product Backlog: GAS End-to-End Implementation

This document tracks the execution of the Gameplay Ability System (GAS) migration for Adventure.

## Epic 1: Foundation & Resources (The Engine)
Goal: Ensure characters initialize with a PlayerState-owned ASC and core attributes.

- [x] **01. PlayerState ASC**
  - Action: Implement `AAdventurePlayerState` with replicated ASC.
  - Status: DONE

- [x] **02. AttributeSet**
  - Action: Implement `UAdventureAttributeSet` with Health, Stamina, RitualEnergy.
  - Status: DONE

- [x] **03. Default Attributes GE**
  - Action: Create `UGE_AdventureDefaultAttributes` to set defaults.
  - Status: DONE

- [x] **04. Ability Stubs + Tags**
  - Action: Implement `UGA_AdventureSprint`, `UGA_AdventureDodge`, `UGA_AdventureTraversal`, `UGA_AdventureInteract`.
  - Status: DONE

- [ ] **05. Hook Default Abilities in Blueprints**
  - Action: Verify DefaultAbilities/DefaultAttributesEffect in both character BPs.
  - Status: PENDING

## Epic 2: Movement Abilities (The Feel)
Goal: Stamina-driven movement abilities (sprint, dodge, jump) via GAS.

- [ ] **06. Create Sprint Cost + Modify Effects**
  - Action: Create `GE_SprintCost` and `GE_SprintModify` (stamina drain, speed change).
  - Status: PENDING

- [ ] **07. Implement GA_Sprint**
  - Action: Ability applies sprint effects and ends on input release.
  - Status: PENDING

- [ ] **08. Implement GA_Dodge**
  - Action: Ability applies stamina cost and triggers montage/root motion or mover action.
  - Status: PENDING

- [ ] **09. Optional GA_Jump**
  - Action: Ability-driven jump cost (if desired for stamina gating).
  - Status: PENDING

## Epic 3: Combat Loop (The Game)
Goal: Ritual Energy gain/spend abilities and core combat.

- [ ] **10. GA_LightAttack / GA_HeavyAttack**
  - Action: Implement melee abilities that grant Ritual Energy on hit.
  - Status: PENDING

- [ ] **11. GA_Parry / GA_Block**
  - Action: Add parry/block abilities with Ritual Energy gain.
  - Status: PENDING

- [ ] **12. GE_RitualEnergyGain**
  - Action: GameplayEffect(s) for +3/+8/+10/+12/+20 gains.
  - Status: PENDING

## Epic 4: UI Binding & Telemetry
Goal: Live HUD binding and testing hooks.

- [ ] **13. Bind HUD to AttributeSet**
  - Action: Bind Health/Stamina/RitualEnergy to UMG.
  - Status: PENDING

- [ ] **14. Debug Hooks**
  - Action: Optional console/debug utilities for testing attribute changes.
  - Status: PENDING

Notes
- GAS is PlayerState-owned; both character variants call InitializeAbilitySystem on possession and replication.
- Mover2 variant uses the same ASC and ability activation paths as the CMC character.
