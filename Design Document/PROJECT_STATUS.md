```markdown
# Project Status — Adventure (concise)

This file provides a condensed, actionable view of the prototype plan and the
current progress. Keep this file updated when tasks complete.

Milestones (prototype, 12 weeks)
- M1 — Playable character + camera modes + RitualEnergy AttributeSet. (Target: Week 2)
- M2 — Two basic abilities + enemy stub (Week 4)
- M3 — Motion-matching prototype integrated (Week 8)
- M4 — Artifact & Boss prototype (Week 10)
- M5 — Playtests & polish (Week 12)

Phase summary (current status)

- Phase 1 — Foundations (Weeks 1–2): IN PROGRESS
  - Project boot & engine config: DONE
  - Input bindings and PlayerController: DONE
  - Camera modes + toggle: PENDING
  - Core locomotion (walk/run/jump/sprint/dodge): IN PROGRESS
  - Vault/mantle & traversal stubs: PENDING
  - AnimBP retarget + montage fallback: PENDING
  - `AS_RitualEnergy` AttributeSet: DONE
  - HUD (WB_HUD) bindings: IN PROGRESS

- Phase 2 — Combat & GAS (Weeks 4–6): PLANNED
  - GA_LightAttack, GA_HeavyAttack: TODO (specs present in Implementation Plan)
  - Enemy grunt AI: TODO

- Phase 3 — Motion-Matching (Weeks 7–9): PLANNED
  - Run `Design Document/motion_matching_pipeline.py` to export manifest and features.
  - Import DB and integrate runtime query (fallback montage supported).

- Phase 4 — Artifacts & Boss (Weeks 10–12): PLANNED
  - Implement `ArtifactData` DataAsset and `GA_Ritual_Signature` ability.
  - Boss prototype using artifact mechanics.

Top priority next actions (short)
1. Complete camera mode toggle and acceptance test (Phase 1). Owner: `engineer/locomotion`.
2. Finalize `WB_HUD` binding and add to PlayerController BeginPlay. Owner: `engineer/ui`.
3. Create issues for `GA_LightAttack` and `GA_HeavyAttack` from task templates. Owner: `engineer/combat`.

4. VS-000 Vertical Slice: branch `feat/vertical-slice/{initials}`, scaffold map and automation smoke test. Owner: `@tech-lead` — Issue: `issues/VS-000_Vertical_Slice_Core.md` (in progress)

Where to track progress
- Convert TODO items into issues in `Documentation/issues/` and link back to this file.
- For urgent migration tasks, update `MIGRATION_CHECKLIST.md` (repo root) and mark progress here.

Notes
- This file is intentionally brief — keep detailed plans in `Adventure_Implementation_Plan.md` and use this file for quick triage and status.

``` 