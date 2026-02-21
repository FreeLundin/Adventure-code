# Adventure — Phase1 TODO

This file mirrors the tracked Phase1 checklist used by the automation helpers.

- [x] Phase1: Project boot & engine config (UE5.7)
- [x] Phase1: Input bindings and PlayerController setup
- [ ] Phase1: Camera modes + toggle implementation
- [ ] Phase1: Core locomotion (walk/run/jump/sprint/dodge)
- [ ] Phase1: Vault/mantle & traversal stubs
- [ ] Phase1: AnimBP retarget + montage fallback setup
- [x] Phase1: Implement AS_RitualEnergy AttributeSet
- [-] Phase1: Basic HUD (Health, RitualEnergy) — in-progress: `WB_HUD` created, bindings pending
- [-] Phase1: Create sample level and integration checkpoints — in-progress: `VS_House` exists
- [ ] Phase1: Fix Up Redirectors and incremental commits
- [ ] Phase1: Acceptance test & playtest checklist
- [-] Phase1: Document work and open issues — in-progress
- [x] Define Vertical Slice idea
- [x] Establish Job Governance document
- [x] Scaffold Phase1 Editor scripts and input config
- [-] Phase1: GA_Climb K2 wiring — in-progress: `DoClimbLogic` graph created; manual wiring required

Next steps (short):
1. In Unreal Editor: open `/Game/Abilities/GA_Climb`, add the K2 nodes per `scripts/ga_climb_k2_plan.json` and my manual recipe in the repo.
2. Finalize `WB_HUD` bindings to `AS_RitualEnergy_BP` and add to PlayerController BeginPlay.
3. Run Fix Up Redirectors, save all assets, commit Editor-created assets.
4. Run PIE acceptance playtest and record issues.

Files of interest:
- `scripts/scaffold_phase1.py`
- `scripts/scaffold_ga_climb.py`
- `scripts/auto_wire_ga_climb.py`
- `scripts/auto_wire_ga_climb_k2.py`
- `scripts/apply_ga_climb_k2_nodes.py`
- `scripts/ga_climb_k2_plan.json`

If you want a different file location or format, tell me where and I will move/convert it.
