# Technical Debt Register

This document tracks known areas of technical debt and their current status.  Close items as they are resolved.

| ID | Area | Description | Status | Notes |
|----|------|-------------|--------|-------|
| TD-001 | Input system | Enhanced input actions unbound in `PC_AdventureController.cpp` | Completed | Actions are now bound in SetupEnhancedInput; input mapping context support added.
| TD-002 | Replication | Character lacks `GetLifetimeReplicatedProps` entries | Completed | Two properties registered (TraversalResult, CharacterInputState).
| TD-003 | Camera transitions | TODO for smooth blending and HUD update | Completed | Smooth blend logic and HUD notifications are implemented in controller.
| TD-004 | Stub headers | Previous `*.generated.h` files accidentally committed | Closed | .gitignore now ignores them.
| TD-005 | DataAsset tooling | Motion-matching pipeline external, no CI check | In progress | Script added (`Automation/motion_matching_pipeline.py`); runbook created; CI hookup pending.  |
| TD-006 | Cultural review | Artifact implementation lacks formal checklist | Open | Pending `runbooks/CULTURAL_REVIEW_CHECKLIST.md`.
| TD-007 | CI coverage | No build/lint on push before today | Closed? | New GitHub Actions workflow added.
| TD-008 | TODO comment proliferation | ~20 TODOs across codebase | Open | Create issues for each major TODO.
| TD-009 | Documentation drift | Long-form plans not synced with repo state | In progress | Ongoing consolidation; README now holds primary design.

*Last updated: 2026‑03‑03*
