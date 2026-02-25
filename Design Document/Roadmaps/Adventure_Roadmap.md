# Adventure Roadmap

Overview: high-level milestones and 2-week sprint plan for the Adventure
prototype (UE5.7). This roadmap focuses on establishing core gameplay, the
GAS ability set, and the motion-matching animation pipeline.

Milestones:

- Prototype (M1, 4 weeks): Core locomotion, basic combat (light/heavy), RitualEnergy attribute, one artifact (Orisha) implemented.
- Motion-Matching (M2, 6 weeks): Create motion database, feature extraction, runtime query, basic blending.
- Encounters & AI (M3, 6 weeks): Basic enemy AI, encounters, boss prototype using artifacts.
- Polish & Metrics (M4, 4 weeks): Playtests, tune energy costs, animation polish, and profiling.

Sprint plan (example 2-week sprint):

1. Sprint A — Core movement + AttributeSet for `RitualEnergy`.
2. Sprint B — Implement `GA_LightAttack`, `GA_HeavyAttack` and basic gameplay effects.
3. Sprint C — Motion-matching pipeline integration: export manifest, import DB, runtime queries.
4. Sprint D — Artifact `DataAsset` + signature ability; visual/audio gameplay cues.

Risks & mitigations:

- Motion-matching complexity: start with a minimal feature set (root velocity + hip/joint snapshots).
- GAS integration learning curve: provide C++/Blueprint templates and examples.
- Asset retargeting: prefer consistent rig naming or use Control Rig retarget maps early.

Owners: assign feature owners per sprint in the issue tracker and link PRs to roadmap items.

Current status (Feb 21, 2026)
- GAS foundation complete (PlayerState ASC, AdventureAttributeSet, tags, ability stubs).
- Default attributes GameplayEffect added.
- Mover2 character variant implemented.
- Enhanced Input assets, camera toggle, and HUD bindings still pending.
