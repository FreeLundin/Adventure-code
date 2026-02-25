```markdown
# Governance — Adventure Documentation & Contribution Guidelines

Purpose: ensure documentation and project work remain discoverable, actionable,
and reviewable. These rules capture lessons learned from the prototype and
define a lightweight process for contributors.

1) Ownership
- Documentation owner(s): add `CODEOWNERS` entries for `Documentation/` pointing to `@design-lead` and `@tech-lead` for technical docs.
- File-level ownership: every major doc or runbook should list an `Owner:` front-matter line or a top-line maintainers note.

2) Index & Status
- Keep `Documentation/INDEX.md` and `Documentation/PROJECT_STATUS.md` current. Small doc changes must include a line in the PR description telling which INDEX or PROJECT_STATUS entry to update.
- `PROJECT_STATUS.md` is the quick triage file — update it when a milestone task moves state (TODO → In Progress → Done).

3) Issues, Backlog and TODOs
- Convert actionable TODO entries into issues under `Documentation/issues/` with required front-matter: `Owner`, `Estimate`, `Priority`, `Phase`.
- Backlog items in `Product_Backlog.md` should map to issues; reference issue IDs in the roadmap and `PROJECT_STATUS.md` for traceability.

3a) Change Tracking (MANDATORY)
- **ALL code, configuration, and asset changes MUST be logged in `CHANGELOG.md`** (project root).
- See `Documentation_Workflow.md` for the complete change tracking process.
- Every session MUST update CHANGELOG.md before completion - no exceptions.
- Configuration (.ini) changes require: setting name, value, why, impact, verification, and rollback procedure.
- Code changes require: file paths, line counts, why needed, dependencies added.
- AI assistants must create/update CHANGELOG entries as work progresses, not retrospectively.

4) PR & Review Rules
- Small docs fixes may merge with one approval from a reviewer listed in `CODEOWNERS`.
- Design or gameplay changes require at least one engineering and one design reviewer, and must reference related issues and the `PROJECT_STATUS.md` entry if milestone-impacting.
- PRs that change behavior (code, DataAssets, scripts) must include a short test or reproduction steps in the PR description.

5) Templates & Automation
- Add and use: `Documentation/.github/ISSUE_TEMPLATE.md`, `Documentation/.github/PULL_REQUEST_TEMPLATE.md`, and a `CODEOWNERS` entry for `Documentation/`.
- Issue template requires fields: `Title`, `Description`, `Acceptance Criteria`, `Estimate`, `Owner`, `Priority`, `Phase`.
- PR template must include: `Summary`, `Related issues`, `Files changed (docs, assets, code)`, `Testing steps`, and `Which PROJECT_STATUS entry updated`.

6) Runbooks & Scripts
- Runbooks in `Runbooks/` must include exact command snippets, expected outputs or artifacts, and a short troubleshooting section. Example: how to run `motion_matching_pipeline.py` from the Editor with a one-line snippet.
- Editor Python scripts in `Documentation/scripts/` must document entry points and required Editor context (e.g., run inside Unreal Editor Python console).

7) Migration and Content Changes
- Use `MIGRATION_CHECKLIST.md` (repo root) for major asset reparenting or redirector fixes and link completed steps back to `PROJECT_STATUS.md`.
- When migrating Blueprints/Assets, include a JSON export sample in the PR to support automated checks.

8) Labels, Branching and Commits
- Branch naming: `feature/<short-desc>`, `fix/<short-desc>`, `chore/<short-desc>`, `docs/<short-desc>`.
- Labels: use `phase1|phase2`, `milestone-M1..M5`, `area:docs`, `area:animation`, `area:gameplay`.
- Commit messages: short summary line + optional body. For large design changes include a one-line decision record reference.

9) Release Notes
- Add per-milestone notes to `Documentation/CHANGELOG.md` summarising user-visible changes and any migration steps.

10) Enforcement & CI
- Lightweight checks: require PR description to reference an issue (or use `docs: trivial` label for obvious fixes). Optionally add a markdown lint CI step.

11) Quick lessons learned
- Keep one concise status file (`PROJECT_STATUS.md`) — it dramatically reduces onboarding friction.
- Convert TODOs to issues with owners/estimates for clear accountability.
- Runbooks need exact commands — vague instructions slow down on-call and handoffs.

These Governance rules are intentionally lightweight. If you want, I can add the `ISSUE_TEMPLATE.md`, `PULL_REQUEST_TEMPLATE.md`, and a `CODEOWNERS` file now.

``` 