# Phase 1 Burndown — Adventure (12-week prototype: Phase 1)

Purpose: break Phase 1 (foundations) into a short, testable burndown so we can reach a reproducible acceptance playtest.

Status update (Feb 21, 2026)
- C++ foundation complete; live coding build succeeds.
- GAS foundation implemented (PlayerState ASC, AdventureAttributeSet, tags, ability stubs).
- Default attributes GameplayEffect added (UGE_AdventureDefaultAttributes).
- Mover2 character variant added (ACBP_AdventureCharacter_Mover).
- Enhanced Input assets, camera toggle, HUD bindings, and animation retarget still pending.

Duration: 2 weeks (Sprint 1), deliverable: playable character with three camera modes, basic HUD, and `RitualEnergy` AttributeSet exposed to UI.

Owners: engineer/locomotion (primary), engineer/ui (HUD), engineer/animation (retarget fallback)

Day-by-day plan (2-week sprint)

- Day 1: Project boot & engine settings
  - Verify project set to UE5.7 in `DefaultEngine.ini`.
  - Enable plugins: `GameplayAbilities`, `GameplayTasks`, `GameplayTags`.
  - Create branch `phase1/foundations` and commit baseline.

- Day 2: Input bindings & PlayerController
  - Add mappings to `Config/DefaultInput.ini` (Move, Look, LMB, RMB, Dash, Interact, CameraToggle).
  - Implement basic `PlayerController` or input component that routes inputs to character.

- Day 3–4: Camera modes implementation
  - Add three camera components (TopDown, ThirdPerson, FirstPerson) to the character blueprint/C++ class.
  - Implement `ToggleCamera()` to cycle modes on mouse wheel; smooth Lerp between TPM and FP.
  - Acceptance: toggle cycles, player input remains functional in each mode.

- Day 5–7: Core locomotion
  - Implement walk/run (sprint), jump, dodge/sprint input, and a simple movement state machine.
  - Wire movement state to AnimBP via an enum or blackboard variable.
  - Acceptance: movement responds to input; states reflected in AnimBP variables.

- Day 8: Vault/mantle & traversal stubs
  - Add simple overlap-based vault/mantle stubs that animate or teleport the player for prototyping.
  - Leave detailed montage or MM integration for later; provide hooks for `AdvancedTraversalSystem` calls.

- Day 9: AnimBP retarget + montage fallback
  - Retarget a minimal set of animations to the project skeleton (or import a small montage fallback).
  - Ensure AnimBP has montage handling for attack/ability placeholders.

- Day 10: `AS_RitualEnergy` AttributeSet
  - Create `AS_RitualEnergy` (C++ or Blueprint) with attribute `RitualEnergy` (float, 0–100).
  - Add small functions to modify the attribute via `AbilitySystemComponent` or direct calls for prototype.

- Day 11: Basic HUD
  - Create UMG widget showing Health and RitualEnergy and bind to AttributeSet values.
  - Add widget to viewport on PlayerController possession.

- Day 12: Sample level & acceptance playtest
  - Build small test map with obstacles and a placeholder enemy (can be a pawn that damages on overlap).
  - Run acceptance playtest checklist (see below).

- Day 13: Fix Up Redirectors, re-save assets, and commit
  - Run Editor `Fix Up Redirectors`, re-save touched assets, then commit changes in small chunks.

- Day 14: Document results and open issues
  - Record parameter values, missing animations, retarget backlog, and next sprint items in `Documentation/`.

Acceptance playtest checklist (Phase 1)

- Camera toggle cycles TopDown → ThirdPerson → FirstPerson and input mapping remains usable.
- Basic movement: walk, sprint, jump, dodge; movement state exposed to AnimBP.
- `RitualEnergy` Attribute exists and changes via test calls (gain/loss) and UI updates.
- Vault/mantle stub works as a visible traversal action.
- HUD shows Health and RitualEnergy values and updates in real time.

Definition of Done (Phase 1)

- All acceptance checklist items pass a short playtest (5–10 minutes) by the owner.
- Implementation committed and pushed to `phase1/foundations` branch.
- Short report written to `Documentation/Phase1_Burndown.md` including blockers and next-sprint tasks.

Burndown KPIs

- Time to first playtest (goal: ≤ 10 working days).
- Number of outstanding animation retarget items (goal: ≤ 5 critical).
- Number of Editor-time manual fixes (redirectors, missing assets) after commit (goal: 0–2).

Test plan (basic)

1) Launch Editor, start Play-In-Editor (PIE) single-player.
2) Verify camera toggle: cycle modes and confirm control mapping in each.
3) Exercise movement inputs: walk, sprint, jump, dodge; confirm AnimBP variables change.
4) Trigger a RitualEnergy gain/loss (use a console exec or debug hotkey) and watch HUD update.
5) Walk over vault test volume to run vault/mantle stub.
6) Note any crashes or Editor warnings; open issues for critical failures.

Runbook snippets

- Open Editor (Windows):
```
"C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor.exe" "C:\Unreal_Projects\Adventure\Adventure.uproject"
```

- Fix Up Redirectors: Content Browser → Right-click top-level Content folder → Fix Up Redirectors

- Run HUD scaffold (example):
```
# In Editor Python console
exec(open(r"<Project>/Scripts/scaffold_ga_climb.py").read())
```

Notes & risks

- Animation gaps are the highest-risk item. Use montage fallback for sprint 1 and capture a retarget backlog.
- Editor Python scripts must be run inside the Editor; I can run them if you open an Editor Python session or want the exact exec commands.
