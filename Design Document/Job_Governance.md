# UE5.7 Job Governance (Jobs-as-Work) — Adventure

Purpose
- Ensure all UE5.7 features implemented through VS Code follow a consistent, auditable, and high-quality workflow using Jobs as the atomic unit of work. This governance model supports AI-assisted development, MCP-driven automation, and multi-developer collaboration.

Roles
- **Product Owner** — Defines feature intent, acceptance criteria, and priority.
- **Technical Lead / Architect** — Validates feasibility, approves job decomposition and subsystem boundaries.
- **Developer** — Executes Jobs via VS Code tasks and MCP actions.
- **Automation Steward** — Maintains `tasks.json`, MCP schemas, GitHub Actions, and CI gates.
- **Reviewer** — Performs code review and ensures compliance with job governance.

Job Definition Standard
Every job is a versionable, auditable unit. Each job MUST include:
- **Job ID** — Unique identifier (e.g., `UE-JOB-Gameplay-001`).
- **Subsystem** — e.g., Gameplay, Rendering, Editor, AI Assistant, Omniverse Bridge.
- **Intent Statement** — One-sentence outcome.
- **Inputs** — Files/assets/schemas/engine states required.
- **Outputs** — Code, Blueprints, assets, tests, docs.
- **Acceptance Criteria** — Testable, measurable, Editor-verifiable.
- **Dependencies** — Other jobs, plugins, modules.
- **Automation Hooks** — VS Code tasks, MCP actions, scripts.

Job Lifecycle
1. **Proposal** — Product Owner drafts job; Architect + Automation Steward review for feasibility.
2. **Decomposition** — Break into atomic tasks; enumerate MCP actions (create class, modify .uproject, run build, open level).
3. **Approval** — Architect signs off on technical design; Reviewer signs off on acceptance criteria.
4. **Execution** — Developer triggers VS Code task; MCP agent performs automated steps; developer completes manual editor steps.
5. **Verification** — Automated tests and Editor-level validation run; Reviewer inspects results.
6. **Merge & Release** — GitHub Actions enforce CI gates; job archived when merged.

VS Code Governance Controls
- **Required Controls**
  - `tasks.json` registry: every job maps to a VS Code task.
  - MCP schema registry: all MCP actions versioned and documented.
  - Subsystem boundaries: jobs must not modify code outside the assigned subsystem without approval.
  - Branching rules: one job = one branch (branch name includes Job ID).
  - Commit message standard: include Job ID and subsystem (e.g., `UE-JOB-Gameplay-001: Add RitualEnergy AttributeSet`).

- **Automation Controls**
  - Pre-commit hooks enforce formatting and linting.
  - GitHub Actions enforce build, unit tests, and editor automation checks where possible.
  - MCP agents log all actions for auditability.

Quality & Testing Requirements
- **Tests per Job**
  - Unit tests for C++ or Python automation.
  - Functional tests for gameplay or editor features.
  - Editor validation (asset loads, blueprint compilation).
  - Performance checks for relevant jobs.

- **Documentation required**
  - Job summary (short document)
  - Code comments and subsystem impact notes
  - Update to subsystem README

Security & Access
- Only **Automation Stewards** may modify `tasks.json` or MCP schemas.
- Only **Architects** may approve subsystem-level changes.
- Jobs modifying engine source require elevated approval and a stricter review path.
- All MCP actions must be sandboxed and logged.

Metrics & Reporting
- Track and report: job cycle time, automation coverage, MCP action usage, subsystem stability, defect rate, developer throughput.
- Use metrics in monthly governance reviews to refine the process.

Enforcement Mechanisms
- GitHub branch protection and required reviewers for subsystem changes.
- CI gates (builds, editor tests) enforced in GitHub Actions.
- VS Code task validation script (recommended) to lint `tasks.json` entries and ensure Job ID/branch naming.

Practical Guidance (Quick checklist for creating a Job)
1. Draft Job in `Documentation/Jobs/UE-JOB-<Subsystem>-NNN.md` with fields from the Job Definition Standard.
2. Add a VS Code task mapping in `/.vscode/tasks.json` referencing the job (Automation Steward approves changes).
3. Create a feature branch named `job/UE-JOB-<Subsystem>-NNN/<short-desc>`.
4. Implement automation hooks (MCP actions, scripts) and reference them in the job doc.
5. Add tests and Editor validation steps; wire CI to run them.
6. Request review, addressing architectural and security approvals.

Repository helpers (suggested files to add)
- `/.vscode/tasks.json` — central registry of Jobs (each task metadata includes `job_id`, `subsystem`, `mcp_schema`).
- `/Tools/validate_tasks_json.py` — simple linter that asserts Job IDs and branch naming conventions.
- `/Documentation/Jobs/` — folder of job proposal markdown files per job.

Strategic Benefit
- Enables natural-language driven development, AI-native automation, and repeatable delivery across Unreal, Omniverse, and Azure tools.
