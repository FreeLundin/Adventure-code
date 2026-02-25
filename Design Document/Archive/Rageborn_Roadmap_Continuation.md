## Adventure Roadmap — Continuation

This document continues the implementation roadmap from `Adventure_Roadmap.md` with short-term milestones, technical dependencies, risks, sprint plan, and owners.

## 📅 Roadmap — Next 3 Months
- **Month 1 — Foundations**: Finalize `Rage` attribute flow, `WBP_HUD`, passive rage generation, and basic replication tests.
- **Month 2 — Combat Systems**: Implement combo windows, `GA_Attack` motion-warping integrations, heavy attack and dash polish, and animation blending fixes.
- **Month 3 — Enemies & Traversal**: Ship `AIC_Grunt` with GAS support, wall-run/double-jump traversal, and finisher/interaction prototypes.

## ⚙️ Technical Dependencies
- **Engine**: UE 5.7 required features (Root Motion, Motion Warping updates).
- **Plugins**: GameplayAbilities, GameplayTags, MotionWarping (project module), Control Rig (optional for authoring).
- **Art**: Combat montages with root motion and matching motion-warp points; VFX palette for Rage mode.

## ⚠️ Risks & Mitigations
- **Animation desync during motion warping**: Create deterministic warp anchors and a replay test to validate timing. Fall back to non-root-motion montage when needed.
- **Network authority for Rage Mode**: Authoritative server-side charge/consume flows via GAS; replicate only attributes and gameplay tags, not cosmetic effects.
- **Hot partitions in Attribute replication (perf)**: Profile attribute update frequency; batch updates or use conditionally replicated gameplay cues.

## 🗓 Sprint Plan (Suggested, 2-week sprints)
- **Sprint 1**: `Rage` attributes, `GE_DefaultStats`, `WBP_HUD`, passive rage generation, basic replication checks.
- **Sprint 2**: `GA_RageMode`, `GE_RageBuff`, visual VFX/FX passes, HUD polish, unit tests for attribute flows.
- **Sprint 3**: Melee combo system, `GA_Attack` motion-warping tie-ins, heavy attack implementation, animation polish.
- **Sprint 4**: AI grunt + GAS integration, finisher interactions, traversal (double jump / wall run), QA & polish.

## 📌 Owners & Notes
- **Core Systems**: `Source/Gameplay` — owner: TBD
- **Animation**: `Content/Animations` — owner: TBD
- **AI**: `Source/AI` + `Content/AI` — owner: TBD
- Add owners and exact milestone dates once team allocation is confirmed.

---

Next steps I can take:
- Insert these sections directly into `Rageborn_Roadmap.md` once the patch tool is functioning, or
- Create GitHub issues from these sprint items and generate a CSV for a sprint board.
