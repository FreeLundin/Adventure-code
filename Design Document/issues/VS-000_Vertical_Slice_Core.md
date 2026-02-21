---
Owner: @tech-lead
Estimate: 5d
Priority: P0
Phase: VerticalSlice
---

# VS-000 — Vertical Slice: Core playable demo

Goal
- Deliver a minimal vertical slice playable in PIE demonstrating core movement (Mover), one enemy AI, one interactive platform, a zipline, and a HUD. Include a local automation smoke test and documentation updates.

Acceptance Criteria
- VS_Playtest.umap loads and is playable in PIE.
- Player movement (Mover-based) responsive and matches example behavior.
- Enemy AI pursues and inflicts damage.
- Moving platform and zipline interactions work.
- Automation smoke test runs locally and asserts basic interactions.
- Documentation updated (`PROJECT_STATUS.md`, `VS_Playtest_README.md`) and `NOTIFY_TEAM.md` includes links to branch/PR.

Tasks
- Create branch `feat/vertical-slice/{initials}` and push early scaffolding commits. (0.5d)
- Scaffold `Content/VS_Playtest/` and `VS_Playtest.umap` placeholder. (0.5d)
- Place `AnimatedMannyPawnExtended` as player pawn and wire Enhanced Input. (1d)
- Implement enemy AI (simple pursue + damage). (1d)
- Add `BP_SimpleMovingPlatform` and `BP_Zipline` interactions. (0.5d)
- Add minimal HUD widget for HP and rage placeholder. (0.5d)
- Add automation smoke test and capture logs. (0.5d)
- Documentation updates and PR creation. (0.5d)

Notes
- Follow `Documentation/HumanInTheLoop_BestPractices.md` when making edits and announcing changes.
- Use branch naming `feat/vertical-slice/{initials}`. Replace `{initials}` with your GitHub handle or initials.
