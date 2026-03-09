```markdown
# Human-in-the-Loop (HITL) Best Practices

Purpose
- Provide concise, practical guidance for efficient, safe, and auditable
  interactions between humans and automation (assistants, scripts, agents)
  during development and content work in this repo.

Principles
- Be explicit: state goals, acceptance criteria, constraints, and priority.
- Keep requests small and testable: prefer short, focused tasks over large
  ambiguous work items.
- Be accountable: assign owners and update `Documentation/PROJECT_STATUS.md`.
- Preserve auditability: record changes, rationale, and reproduction steps.

Requester responsibilities (human)
- Provide: goal, acceptance criteria, relevant file paths, constraints, and
  expected artifacts (examples or outputs).
- Permit actions: state whether the assistant may edit files, create issues,
  or run scripts. If edits are allowed, indicate commit/PR expectations.
- Prioritize: mark critical vs. nice-to-have; give deadlines if relevant.
- Give feedback: mark results as `accepted`, `needs change`, or `blocked`.

Assistant responsibilities (automated or human helper)
- Confirm scope before edits if tasks are ambiguous.
- Use small, reversible changes; prefer adding files or updates rather than
  refactoring large areas without approval.
- Track work via the project's todo/issue workflow and update `PROJECT_STATUS.md`.
- Provide reproducible steps and, where applicable, example commands to test
  changes locally or in the Editor.

Communication template (REQUEST_TEMPLATE)
- Goal: short one-line description and acceptance criteria (pass/fail).
- Files: list file paths or assets to inspect or edit.
- Constraints: things not to change, platform, or performance targets.
- Priority: P0/P1/P2 and deadline (if any).
- Permissions: allow/deny edits, PR creation, running scripts.
- Example:

```
Goal: Make camera toggle functional. Acceptance: mouse wheel cycles Top/TP/FP.
Files: Source/Player/MyPlayerController.cpp, Content/UI/WB_HUD.uasset
Constraints: Do not change input mappings in DefaultInput.ini
Priority: P0, Due: 2026-02-20
Permissions: May edit files and create PR, do not run builds.
```

Change control and audit
- Require a short test plan or repro steps in PRs that affect runtime behavior.
- Add a line to `Documentation/PROJECT_STATUS.md` for milestone-impacting changes.
- For large edits, open an issue first, link design notes, and get approvals.

Security & privacy
- Never include secrets, private keys, or production credentials in requests
  or file edits. Point to safe vaults or instructions for retrieving secrets.
- For any requested data sharing, label sensitivity and redact as needed.

Runbooks & reproducibility
- Assistant should provide exact commands (terminal or Editor Python) to
  reproduce results. Example: how to run a pipeline script in the Editor.

Example minimal workflow
1. Requester creates a small issue using the `REQUEST_TEMPLATE` (or asks via chat).
2. Assistant confirms scope and adds a TODO entry.
3. Assistant applies a small patch, runs quick checks, and creates a PR/stub.
4. Requester reviews, provides `accepted` or requests changes; assistant iterates.
5. After merge, assistant updates `PROJECT_STATUS.md` and closes the TODO.

Checklist for requesters (before asking)
- Have I stated the goal and acceptance criteria?
- Have I listed the exact file(s) or assets to inspect?
- Did I state constraints and whether edits are allowed?
- Did I set a priority or deadline?

This document is intentionally short — keep requests structured and small,
and use the repo's issue system for long-running or invasive changes.

``` 