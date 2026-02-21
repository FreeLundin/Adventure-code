# Adventure — Product Vision

Purpose
- Deliver a focused, prototype-quality action-adventure experience that blends kinetic traversal and combat with ritual-driven supernatural mechanics in a moody, story-rich setting (Old Nola).

Target audience
- Single-player PC players who enjoy exploration-forward action games with atmospheric storytelling, collectible progression, and skill-based combat (players of Tomb Raider, Sekiro-lite, and occult-themed action games).

Value proposition
- Combine tight, satisfying traversal and combat with a layered resource (Ritual Energy) and artifact-driven abilities that provide tactical choices in encounters and puzzle-like boss phases.

Key pillars
- Traversal & Discovery: parkour-lite traversal (vault, mantle, climb, swing) that rewards exploration.
- Combat & Rituals: GAS-driven abilities powered by Ritual Energy, mixing firearms/melee with occult artifacts and signatures.
- Narrative & Artifacts: collectible Artifacts (The Seven) that unlock new mechanics and shape boss encounters.
- Animation Quality: motion-matching pipeline as a long-term goal; montage fallback for prototype polish.

MVP / Prototype Scope (12 weeks)
- Playable character with three camera modes (TopDown/Third/First).
- `RitualEnergy` AttributeSet wired to HUD and at least two abilities (light/heavy) that consume energy.
- One artifact implemented with passive + signature ability and a demo boss encounter.
- Small sample level for acceptance playtests and iteration.

Success metrics
- Playtest pass: prototype functional and playable for 10-minute scripted run without critical bugs.
- Retarget backlog reduced to ≤ 5 critical animation issues for prototype actions.
- Ability/system stability: abilities activate, consume RitualEnergy, and do not cause replication or crash errors in local PIE.

Constraints & assumptions
- Platform: PC intended first (UE5.7). Mobile is out-of-scope for the prototype.
- Timeline: prototype focused on vertical slices; motion-matching is high value but scoped after core gameplay.
- Cultural sensitivity: artifacts and inspiration drawn from the Seven African Powers must be handled respectfully; include cultural review and credits in delivery.

Strategic next bets
- Short term: stabilize GAS + RitualEnergy flow and produce a playable boss demo using one artifact.
- Medium term: integrate motion-matching for more fluid traversal and combat animation fidelity.

Ownership & next steps
- Owner: Product / Technical Producer + leads for Locomotion, Combat, Animation.
- Next immediate actions: finalize Phase 1 burndown, scaffold `AS_RitualEnergy` and HUD, and run first 10-minute acceptance playtest.
