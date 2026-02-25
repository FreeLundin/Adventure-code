---
Owner: @tech-lead
Estimate: 2d
Priority: P1
Phase: phase1
---

# Rage Generation — GA_PassiveRage

Create `GA_PassiveRage` ability that listens for `GameplayEvent.DealtDamage` and grants Rage per hit.

Acceptance Criteria
- `GA_PassiveRage` registers for damage events and increases `Rage` by configured amount.
- Behavior validated in a small test level with a damage event trigger.

Related: `Rageborn_Roadmap.md` Phase 1
