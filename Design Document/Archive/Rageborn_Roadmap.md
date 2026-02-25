# Rageborn Gameplay Implementation Plan
*Based on GDD version 1.0 "End Cycle"*

This roadmap converts the existing `GameAnimationSample` GAS foundation into the mechanics required for **Rageborn**.

## 🔴 Phase 1: The Rage Engine (Core Identity)
*Goal: Establish the resource loop defined in the GDD.*
1.  **[DONE] Rage Attributes**: Added `Rage` and `MaxRage` to C++ Attribute Set.
2.  **[ ] Rage Visualization**: Create `WBP_RageBar`.
3.  **[ ] Rage Generation**: Create `GA_PassiveRage`. Logic: Listen for `GameplayEvent.DealtDamage`. Add +5 Rage per hit.
4.  **[ ] Rage Mode**: Create `GA_RageMode`. Cost: 100 Rage. Effect: Apply `GE_RageBuff` (Damage x 2, Speed x 1.5, Screen Filter Red).

## ⚔️ Phase 2: Visceral Combat (Movement & Attack)
*Goal: "Fast, brutal combat with cinematic flair".*
1.  **[ ] Motion Warping Basics**: Configure `GA_Attack` to use the `MotionWarping` component (Code exists, needs BP impl).
2.  **[ ] Combo System**: Implement a "Combo Window" using GAS Gameplay Events (`Event.Combat.ComboWindow`).
3.  **[ ] Heavy Attack**: Create `GA_GroundSlam`. Logic: Jump Z + 500, then timeline smash down -> Apply Radial Damage.
4.  **[ ] Execution**: Create `GA_Finisher`. Requirement: Target Health < 20. Logic: Play synced interaction montage.

## 🏃 Phase 3: Traversal (Superhuman Movement)
*Goal: "Sprint, double jump, wall-climb".*
1.  **[DONE] Sprint**: `GA_Sprint` implemented.
2.  **[DONE] Dash**: `GA_Dash` implemented with Cooldown.
3.  **[ ] Double Jump**: Update `GA_Jump`. Logic: Track `JumpCount` int. If Air + JumpCount < 2, Launch Character + Play Air Anim.
4.  **[ ] Wall Run**: Create `GA_WallRun`. Logic: Line trace sides -> Get Surface Normal -> Set Gravity 0 -> Set Velocity along wall.

## 🎯 Phase 4: Enemy AI & Interaction
*Goal: "Waves of enemies in reactive combat".*

1. **[ ] AI Core**: Create `AIC_Grunt` + `BP_Grunt`.
2. **[ ] Target System**: Create `UAISense_Sight`.
3. **[ ] GAS AI**: Give AI the `AttributeSet`. They need Health too.
4. **[ ] Behavior Tree**: Patrol -> See Player -> Approch -> Ability Use (`GA_Melee`).

## 🛠️ Immediate Next Steps (Day 1)
1.  Initialize Rage (0/100) in `GE_DefaultStats`.
2.  Create `WBP_HUD` to see Health/Stamina/Rage.
3.  Create the "Rage Generation" passive ability.
