# SVGLND — REQUEST TEMPLATE (Spec-Driven Development)

Use this template for every IDE-executed task.

## Metadata
- Project: SVGLND
- Task ID: (e.g., M3-T1)
- Priority: P0/P1/P2
- Owner: Brian-Christopher Lundin
- Status: Proposed / Active / Done / Blocked

## Goal
One sentence describing the outcome.

## Acceptance Criteria (Pass/Fail)
- [ ] Condition 1 (testable)
- [ ] Condition 2 (testable)

## Files / Assets
List explicit paths. No path = no edits.

## Constraints (Non-Negotiable)
- ATS owns locomotion/traversal; do not modify ATS plugin.
- Niagara only for VFX (Niagara Examples patterns).
- MetaSounds only for gameplay audio (no SoundCues).
- GAS governs abilities/tags; StateTree governs orchestration only.

## Permissions
- Allowed: code edits / config edits / docs edits / run tests
- Forbidden: plugin edits / content graph wiring unless MCP

## Test Plan (PIE/Automation)
1) Step
2) Expected result

## Rollback Plan
How to revert if it fails.
