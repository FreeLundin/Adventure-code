# Adventure Product Backlog

This backlog organizes features, technical work, and QA tasks for Adventure. Items are grouped by Epic and prioritized for the prototype horizon. Use this as the source of truth for creating issues and assigning owners.

Communication & issue standards (new)
- All backlog entries must be converted to issues using the `Documentation/.github/ISSUE_TEMPLATE.md` or the `Documentation/REQUEST_TEMPLATE.md` format. Each issue MUST include the following fields in its front-matter or body: `Owner`, `Estimate`, `Priority`, `Phase`, and `Acceptance Criteria`.
- Link the created issue back to `Product_Backlog.md` (copy the issue ID next to the backlog entry) and reference the related `PROJECT_STATUS.md` milestone if the work is milestone-impacting.
- Use the Human-in-the-Loop guidance in `Documentation/HumanInTheLoop_BestPractices.md` for structuring requests where you expect an assistant to act (edit files, run scripts, or create PRs).

How to use:
- Each backlog item below should become an issue with: Title, Description, Acceptance Criteria, Estimate, Owner, Priority label, and `Phase` tag.

Epic: Core Foundations
- PF-001: Project setup & UE5.7 configuration
  - Priority: High
  - Description: Configure engine, enable GAS-related plugins, baseline project settings, CI notes.
  - Acceptance: Project opens in UE5.7; required plugins enabled.

- PF-002: Input system & PlayerController
  - Priority: High
  - Description: Default input mappings and PlayerController that routes inputs to character and camera toggles.
  - Acceptance: Mapped inputs appear in `DefaultInput.ini`; PlayerController handles inputs in PIE.

- PF-003: Camera system (TopDown/Third/First)
  - Priority: High
  - Description: Camera components, toggle logic, smooth transitions and mode-specific control tuning.
  - Acceptance: Player can cycle camera modes with mouse wheel; controls remain usable.

- PF-004: Character locomotion
  - Priority: High
  - Description: Walk, run, sprint, jump, dodge; movement state machine and AnimBP linkage.
  - Acceptance: Movement actions respond; states seen in AnimBP.

Epic: GAS & Combat
- GC-001: `AS_RitualEnergy` AttributeSet
  - Priority: High
  - Description: AttributeSet for RitualEnergy (0–100) with read/write API for abilities and effects.
  - Acceptance: Attribute exists and updates; HUD binding confirmed.

- GC-002: GA_LightAttack & GA_HeavyAttack
  - Priority: High
  - Description: Two basic abilities that consume RitualEnergy via GameplayEffects.
  - Acceptance: Abilities activate, apply effects, and consume RitualEnergy.

- GC-003: GA_Climb (ATS integration)
  - Priority: Medium
  - Description: Map ATS climb behavior to a GameplayAbility and wire AnimNotifies to GameplayEvents.
  - Acceptance: Climb ability performs montage and affects movement state.

Epic: Animation
- AN-001: AnimBP retargeting & montage fallback
  - Priority: High
  - Description: Retarget key animations and ensure montage fallback for sprint 1.
  - Acceptance: Animation fallback plays for core actions; retarget backlog captured.

- AN-002: Motion matching prototype
  - Priority: Medium
  - Description: Export manifest, extract features for a minimal set, import DB, and integrate runtime query.
  - Acceptance: Motion matching returns plausible poses for sample actions and improves transitions.

Epic: Assets & Migration
- AM-001: ATS asset migration
  - Priority: High
  - Description: Bring `AdvancedTraversalSystem` assets into `Content/AdvancedTraversalSystem`, fix redirectors, adapt refs.
  - Acceptance: Assets tracked in git and referenced correctly by maps/blueprints.

Epic: Systems & Tools
- ST-001: Editor Python scaffolds for GAS assets
  - Priority: Medium
  - Description: Scripts to create placeholder `GA_` and `AS_` assets and produce mapping JSON for ATS→GAS.
  - Acceptance: Scripts run inside Editor and create expected placeholders.

- ST-002: Test harness & sample level
  - Priority: Medium
  - Description: Small level for acceptance tests with spawn points, debug triggers, and basic enemy stub.
  - Acceptance: Level supports automated short playtest scenarios.

Epic: Polish & QA
- QA-001: Acceptance playtest checklist & telemetry
  - Priority: High
  - Description: Define playtest steps and lightweight telemetry for RitualEnergy and ability activations.
  - Acceptance: Playtest passes and logs captured.

Backlog grooming notes
- Tag items with `phase1`, `phase2`, `motion-matching`, or `polish` to map to roadmap milestones.
