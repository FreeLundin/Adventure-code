# Adventure Implementation Plan

Purpose: break the `Adventure_Roadmap.md` milestones into concrete tasks, owners, deliverables,
and an incremental schedule suitable for a 3-month prototype (UE5.7) aligned with the canonical GDD.

Overview
- Prototype horizon: 12 weeks (3 months) divided into four 3-week phases (or six 2-week sprints).
- Core identity: Jessie "Nola" Savage explores Old Nola, battling occult enemies using hybrid combat (firearms/melee/artifacts), managing Ritual Energy (0-100), and collecting artifacts tied to the Seven African Powers (Orishas).
- Primary goals: multi-perspective camera (TopDown/TP/FP via mouse wheel), Ritual Energy + GAS pipeline, motion-matching integration, artifact system with Seven Orishas, one multi-phase boss with ritual puzzle mechanics.

Current status (Feb 21, 2026)
- GAS foundation complete in C++ (PlayerState-owned ASC, AdventureAttributeSet, gameplay tags, ability stubs).
- Default attributes GameplayEffect added (UGE_AdventureDefaultAttributes).
- Mover2 character variant added (ACBP_AdventureCharacter_Mover).
- Live coding build succeeds.

Milestones & Phases

- Phase 1 — Foundations (Weeks 1–3)
  - Tasks: UE5.7 project setup, input system + camera toggle (mouse wheel: TopDown/ThirdPerson/FirstPerson), core movement (walk/sprint/jump/vault/mantle), basic HUD (Health, RitualEnergy prominent, Stamina, Ammo), RitualEnergy AttributeSet (0-100 with decay -1/sec after 8s), Mover2 variant validation.
  - Deliverables: Playable Jessie character with 3 camera modes functional; AttributeSet implemented with decay and exposed to HUD; Mover2 pawn variant functional; small animation set retargeted to project skeleton.
  - Owner: `engineer/locomotion`
  - Acceptance: Camera toggle works via mouse wheel, HUD shows Ritual Energy prominently, character can vault/mantle basic obstacles.

- Phase 2 — Combat & GAS (Weeks 4–6)
  - Tasks: Implement GAS abilities: `GA_LightAttack` (+3 Ritual Energy, costs 0), `GA_HeavyAttack` (+8 RE), `GA_Parry` (+12 RE), `GA_Block`, `GA_Dodge`. Create `GameplayEffect` cost/gain patterns via GAS. Implement firearms (limited ammo, uses Ritual Energy for special shots). Build simple enemy AI (Grunt cultist) with Behavior Tree (Patrol/Attack states).
  - Deliverables: Core combat abilities working with Ritual Energy flow validated; at least two enemy types (Grunt, Cultist) with attack patterns; stealth takedown prototype (+10 RE).
  - Owner: `engineer/combat`
  - Acceptance: Light/heavy attacks gain Ritual Energy per GDD values; parry system functional; enemies respond to player actions; Ritual Energy HUD updates in real-time.

- Phase 3 — Motion-Matching & Anim Integration (Weeks 7–9)
  - Tasks: Run motion-matching pipeline (manifest export using `Design Document/motion_matching_pipeline.py`, per-frame feature extraction: root velocity, joint velocities, contact flags), build CSV manifest, index runtime DB, implement AnimGraph integration with runtime query & blending. Montage fallback for uncovered cases (rope-swing, special attacks).
  - Deliverables: Motion-matching prototype improving transition smoothness for common actions (walk/run/jump/vault); documentation of pipeline steps in runbook; fallback montages for ritual animations.
  - Owner: `engineer/animation`
  - Acceptance: Motion-matching DB contains at least 10 animation clips; runtime query reduces snapping on locomotion transitions; playtest confirms subjective improvement.

- Phase 4 — Artifacts & Boss (Weeks 10–12)
  - Tasks: Implement `ArtifactData` DataAsset system with fields (Name, Orisha, BaseEffects, SignatureModifier, UpgradeTiers). Create at least TWO artifacts from the Seven (recommend: Yemaya - Oceanic Chalice for healing/cleanse, Shango - Thunderbrand for damage/lightning). Implement `GA_Ritual_Signature` (costs 100 RE, modifiable by equipped artifact). Build multi-phase boss encounter integrating ritual puzzle mechanics (e.g., sigil decoding, component placement) + environmental weaknesses tied to artifact abilities.
  - Deliverables: Working artifact system with passive GameplayEffects + Signature abilities; two artifacts with distinct mechanics; boss encounter demo level with 2-3 phases requiring artifact use; ritual puzzle prototype (sigil/rhythm mechanic).
  - Owner: `engineer/encounters`
  - Acceptance: Artifacts grant passive bonuses and modify Signature Ritual; boss encounter requires artifact mechanics to progress through phases; cultural review checklist completed for Orisha representation.

Sprint Breakdown (example, 2-week sprint cadence)
- Sprint 1 (Weeks 1-2): Phase 1 core movement, camera toggle implementation (mouse wheel), HUD stub with Ritual Energy bar.
- Sprint 2 (Weeks 3-4): Continue movement polish (vault/mantle), `AS_RitualEnergy` with decay logic, animation retargets for locomotion.
- Sprint 3 (Weeks 5-6): Light/Heavy/Parry attacks with GAS, GameplayEffect costs/gains, Ritual Energy flow tuning per GDD values (+3/+8/+12).
- Sprint 4 (Weeks 7-8): Enemy AI (Grunt + Cultist with Behavior Trees), basic encounter design, stealth takedown (+10 RE), tune Ritual Energy gains.
- Sprint 5 (Weeks 9-10): Motion-matching manifest export & initial DB import, runtime query hook, montage fallbacks for ritual animations.
- Sprint 6 (Weeks 11-12): Artifact DataAsset implementation (Yemaya + Shango), Signature ability system, multi-phase boss prototype with ritual puzzle, final playtest & polish.

Task Templates (copy into issue tracker)

- Title: `CAM-001: Implement Multi-Perspective Camera Toggle`
  - Description: Create camera component supporting TopDown/ThirdPerson/FirstPerson modes. Mouse wheel cycles through modes with smooth transitions. Each mode tuned for specific use: TopDown (traversal/puzzle), ThirdPerson (combat/exploration), FirstPerson (immersion/detail inspection).
  - Acceptance: Mouse wheel cycles camera modes; each mode has appropriate FOV and offset; transitions are smooth (<0.5s blend).

- Title: `RE-001: Implement Ritual Energy AttributeSet with Decay`
  - Description: Create `AS_RitualEnergy` with float attribute (0-100 range). Implement passive decay (-1/sec after 8s out of combat). Expose to HUD with prominent bar visualization showing current value and decay state.
  - Acceptance: Ritual Energy decays correctly; combat prevents decay; HUD shows real-time updates; decay starts after 8s out of combat.

- Title: `MM-001: Export Animation Manifest for Motion-Matching`
  - Description: Use `motion_matching_pipeline.py` to list asset paths, retarget notes, export per-frame features (root velocity, hip position, contact flags). Produce CSV manifest for 10+ sample clips covering locomotion (walk/run/jump/vault).
  - Acceptance: CSV manifest present with feature schema; per-clip exports exist; documentation updated with pipeline steps.

- Title: `GAS-001: Implement GA_LightAttack with Ritual Energy Gain`
  - Description: Create C++/BP ability for light attack. On hit, apply GameplayEffect granting +3 Ritual Energy. Tag with `ability.attack.light`. Integrate with animation montage and gameplay cue.
  - Acceptance: Ability activates on input, grants +3 RE on successful hit, triggers visual/audio cue, respects ability cooldown.

- Title: `ART-001: Implement Yemaya Artifact DataAsset`
  - Description: Create `ArtifactData` DataAsset for Yemaya (Oceanic Chalice). Passive: life-on-hit effect (small heal per hit). Signature: costs 100 RE, spawns healing tide area, cleanses debuffs. Include cultural note citations and respectful representation review.
  - Acceptance: DataAsset populated with all fields; passive effect applies on equip; Signature ability functional and costs 100 RE; cultural review checklist completed.

- Title: `BOSS-001: Multi-Phase Boss Encounter Prototype`
  - Description: Create boss blueprint with 2-3 phases. Phase 1: standard combat. Phase 2: ritual puzzle (sigil decoding or component placement) requiring artifact ability to interrupt. Phase 3: enraged combat with environmental weaknesses. Boss health gates phase transitions.
  - Acceptance: Boss transitions between phases; ritual puzzle mechanic functional; artifact Signature required to progress; encounter completable in 5-10 minutes.

Dependencies & Risks
- Dependency: consistent skeleton naming and retarget map for motion-matching — mitigate by establishing Control Rig retarget maps in Sprint 1; document skeleton hierarchy.
- Risk: GAS integration bugs with Ritual Energy flow — mitigate by shipping small example abilities with test harness in `Source/AdventureTests` or staged sample level; instrument UE_LOG for all RE gain/decay events.
- Risk: Camera mode switching feels jarring — mitigate with blend curves and FOV interpolation; playtest each mode's use case separately.
- Risk: Artifact system complexity (7 Orishas) — phase implementation: start with 2 artifacts (Yemaya + Shango), expand in post-prototype if time allows.
- Risk: Cultural sensitivity with Orisha representation — require cultural review checklist for each artifact; cite sources in code comments; consider consulting with cultural advisors.
- Risk: Motion-matching pipeline integration time — allocate 2 full weeks in Phase 3; maintain montage fallback system as safety net.

QA & Playtest
- Instrument Ritual Energy events with UE_LOG category `LogRitualEnergy` for telemetry (gains, decays, ability costs).
- Weekly playtest sessions after each sprint; collect crash reports, performance metrics, and short issues list.
- Track playtest feedback on:
  - Camera mode usability (does each mode serve its purpose?)
  - Ritual Energy flow feel (is 100 RE achievable? Too fast/slow?)
  - Artifact Signature impact (does it feel powerful and worth the cost?)
  - Boss encounter clarity (are ritual puzzle mechanics telegraphed clearly?)
- Vertical slice validation in Week 6: ensure 3-5 minute playthrough is smooth and representative.

Tooling & Runbook
- Motion-matching: run `Design Document/motion_matching_pipeline.py` inside Unreal Editor Python to produce `MotionMatchingManifest.csv`.
- Build: use UE `Build.bat` tasks already present for platform builds. For Editor iteration, use `LaunchEditor` or IDE debug.

Acceptance Criteria (project-level)
- Multi-perspective camera toggle (TopDown/ThirdPerson/FirstPerson) works via mouse wheel with smooth transitions.
- Ritual Energy system (0-100, decay -1/sec after 8s) implemented with prominent HUD display and validated gain/cost flow per GDD (+3/+8/+12/+20 values).
- At least FOUR GAS abilities implemented and tested: Light/Heavy attacks, Parry, and Signature Ritual.
- Two artifacts from the Seven implemented with DataAsset architecture, passive effects, and Signature modifiers (recommend: Yemaya + Shango).
- Motion-matching provides measurable improvement in transition smoothness for prioritized actions (qualitative acceptance in playtests); montage fallback operational.
- One multi-phase boss encounter implemented and reproducible in demo map, integrating ritual puzzle mechanics and artifact Signature usage.
- Cultural review checklist completed for all Orisha representations with citations documented.

Next Steps
- Create issues from Task Templates and assign owners.
- Add short-run CI checks (linting, DataAsset JSON exports) and attach to PR template.
