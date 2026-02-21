# Adventure Product Roadmap

Scope: 3-month prototype (12 weeks) focusing on core systems, motion-matching integration, and an artifact-driven boss encounter.

Timeline (12 weeks)

- Weeks 1–2 (Phase 1 — Foundations)
  - Project boot, input, camera modes, core locomotion, `AS_RitualEnergy`, basic HUD, sample level.

- Weeks 3–4 (Phase 2 — Combat & GAS)
  - Implement `GA_LightAttack`, `GA_HeavyAttack`, cost `GameplayEffects`, basic enemy AI, RitualEnergy tuning.

- Weeks 5–8 (Phase 3 — Motion-Matching)
  - Motion-matching manifest export, feature extraction, DB import, runtime query & blend; fallback montages continued.

- Weeks 9–10 (Phase 4 — Artifacts & Encounters)
  - Implement `ArtifactData` and one artifact; signature ability and boss encounter prototype that uses artifact mechanics.

- Weeks 11–12 (Phase 5 — Polish & Playtests)
  - Playtests, tuning, retargeting remaining animations, performance profiling, and bug fixes; create handoff notes.

Milestones

- M1 (End Week 2): Playable character with 3 camera modes, RitualEnergy AttributeSet, HUD, and sample level.
- M2 (End Week 4): Two functional abilities with cost and simple enemy; RitualEnergy validated in combat.
- M3 (End Week 8): Motion-matching prototype integrated and improving transition behavior for core actions.
- M4 (End Week 10): Artifact implemented and boss encounter demonstration.
- M5 (End Week 12): Prototype stabilised and documented; acceptance playtest passed.

Quarterly Roadmap (stretch)

- Q2: Expand ability set, integrate online analytics, and a second artifact.
- Q3: Additional levels and encounters, polish motion matching, and begin mobile feasibility analysis.

Dependencies & risk register

- Motion-matching depends on retarget quality — mitigate with montage fallback and early retarget mapping.
- GAS abilities require a stable AttributeSet model; lock `RitualEnergy` API early and keep changes backward-compatible.
- Asset migration may introduce redirectors and broken refs — schedule dedicated cleanup days after large commits.

How to map backlog → roadmap

- Label backlog issues with sprint tags (`sprint-1`, `sprint-2`, etc.) and milestone references (M1..M5).
- Triage weekly and move highest-value items into the next sprint board.

Monitoring & acceptance

- Weekly playtest notes and short reports for each milestone.
- Track KPIs: playtest pass rate, crash count, and retarget backlog size.
