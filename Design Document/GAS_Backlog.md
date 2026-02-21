# Product Backlog: GAS End-to-End Implementation

This document tracks the execution of the Gameplay Ability System migration.

## Epic 1: Foundation & Resources (The Engine)
*Goal: Ensure the character functions physically with resource constraints.*

- [x] **01. Update Initialization**
    - [ ] Action: Edit `GE_DefaultStats` (Content/GAS).
    - [ ] Task: Add Modifiers: `Stamina = 100`, `MaxStamina = 100`.
- [ ] **02. Create Jump Cost**
    - [ ] Action: Create `GE_JumpCost` (Content/GAS).
    - [ ] Task: Duration: `Instant`. Modifier: `Stamina` `Add` `-10.0`.
- [ ] **03. Link Jump Cost**
    - [ ] Action: Edit `GA_Jump`.
    - [ ] Task: Set "Cost Gameplay Effect Class" to `GE_JumpCost`.
- [ ] **04. Create Sprint Speed Effect**
    - [ ] Action: Create `GE_SprintModify`.
    - [ ] Task: Duration: `Infinite`. Modifier: `CharacterMovement.MaxWalkSpeed` `Multiply` `1.5` (Note: Ensure MaxWalkSpeed attribute exists or simulate via loose tags). 
    - *Correction:* Since GAS Attributes for movement don't exist yet, we will use a "Gameplay Tag" detection in the character tick, OR a simpler approach: Have the Ability simply Change the Walk Speed on Activation, and Reset it on End.
- [ ] **05. Create Sprint Cost Effect**
    - [ ] Action: Create `GE_SprintCost`.
    - [ ] Task: Duration: `Has Duration` (or instant w/ period). Best Practice: Duration `Infinite`, Period `0.5s`. Modifier: `Stamina` `Add` `-5.0`.
- [ ] **06. Create Sprint Ability**
    - [ ] Action: Create `GA_Sprint`.
    - [ ] Logic:
        - On Activate: Check Stamina > 0. Apply `GE_SprintModify` and `GE_SprintCost`.
        - Wait for Input Release (Shift).
        - On Release/Stamina<=0: Remove Effects. End Ability.
- [ ] **07. Sprint Input**
    - [ ] Action: `CBP_GAS_Character`.
    - [ ] Task: Bind Shift -> Try Activate `GA_Sprint`.
- [ ] **08. Regen Effect**
    - [ ] Action: Create `GE_Regen`.
    - [ ] Task: Duration: `Infinite`. Period `1.0s`. Modifier: `Stamina` `Add` `+5.0`.
- [ ] **09. Passive Ability**
    - [ ] Action: Create `GA_PassiveRegen`.
    - [ ] Task: "Net Execution Policy": `Server Only`. On Activate: Apply `GE_Regen`.
- [ ] **10. Regen Logic**
    - [ ] Task: Add to `DefaultAbilities` in Character.

## Epic 2: Advanced Movement (The Feel)
*(To be detailed after Epic 1 is complete)*

## Epic 3: Combat Loop (The Game)
*(To be detailed after Epic 2 is complete)*
