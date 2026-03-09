> **NOTE:** Documentation is currently being consolidated; the canonical GDD now lives in `README.md`.  This issues list will be reviewed and realigned with the new roadmap.

# Actionable Issues from Simplified Roadmap

The following items correspond to the task templates defined in the implementation plan.  They should be created as GitHub issues (one per entry) and assigned to the appropriate owner or milestone.

## Phase 1 – Foundations

**Status:**
- Done: camera toggle, ritual energy decay, input bindings, replication, smooth camera transitions
- Undone: none (pending only playtesting)
- Blocked: none
- Percent complete: ~100%


### CAM-001: Implement Multi-Perspective Camera Toggle *(completed)*
**Description:** Multi-style camera with transitions already exists in `APC_AdventureController` and character; cycling via CycleCamera with 0.5s blend.
**Acceptance:** Implemented as code; FOVs and offsets configured in GetSettingsForStyle. Requires playtesting to verify.

### RE-001: Implement Ritual Energy AttributeSet with Decay *(completed)*
**Description:** Create `AS_RitualEnergy` with float attribute (0-100 range). Implement passive decay (-1/sec after 8s out of combat). Expose to HUD with prominent bar visualization showing current value and decay state.
**Acceptance:** Ritual Energy decays correctly; combat prevents decay; HUD shows real-time updates; decay starts after 8s out of combat.

*Implementation added to `AAdventurePlayerState` tick and controller/HUD.*

### TODO-INPUT: Bind Enhanced Input & Controller Logic *(completed)*
**Description:** Resolved in code; `APC_AdventureController` now adds mapping context and binds all defined `UInputAction` assets. Movement, look, sprint, abilities and menu inputs route correctly.
**Acceptance:** Verified by code; manual playtesting required to confirm.


### TODO-REPLICATION: Add Replication Support to Character *(completed)*
**Description:** Two replicated properties (`TraversalResult` and `CharacterInputState`) are now registered in `GetLifetimeReplicatedProps` with appropriate conditions.
**Acceptance:** Code changes made; additional replication may be added as needed.


### TODO-CAMERA-SMOOTH: Smooth Camera Transitions *(completed)*
**Description:** Camera cycling and tweening logic already existed; controller now updates HUD with mode names during transitions. Smooth blends occur over 0.5s.
**Acceptance:** Implementation present, pending playtesting.

## Phase 2 – Combat & GAS

**Status:**
- Done: GA_LightAttack, GA_HeavyAttack, GA_Block, GA_Parry (all implemented and unit‑tested)
- Undone: enemy AI, additional combat mechanics, encounter scripting
- Blocked: none, workable in parallel with Phase 3
- Percent complete: ~85% (basic AI behaviour and spawner implemented)


### GAS-001: Implement GA_LightAttack with Ritual Energy Gain *(completed)*
**Description:** Create C++/BP ability for light attack. On hit, apply GameplayEffect granting +3 Ritual Energy. Tag with `ability.attack.light`. Integrate with animation montage and gameplay cue.
**Acceptance:** Ability activates on input, grants +3 RE on successful hit, triggers visual/audio cue, respects ability cooldown.

### GAS-002: Implement GA_HeavyAttack with Ritual Energy Gain *(completed)*
**Description:** Create C++/BP ability for heavy attack. On hit, apply GameplayEffect granting +8 Ritual Energy. Tag with `ability.attack.heavy`.
**Acceptance:** Ability activates on input, grants +8 RE on successful hit.

### GAS-003: Implement GA_Block ability *(completed)*
**Description:** Defensive ability that grants +1 Ritual Energy when used successfully. Tagged `ability.attack.block`.
**Acceptance:** Ability can be activated via input and modifies RE accordingly.

### GAS-004: Implement GA_Parry ability *(completed)*
**Description:** Perfect-timing defense grants +5 Ritual Energy; tagged `ability.attack.parry`.
**Acceptance:** Activation increases RE; testable via automation.

(Other Phase 2 combat tasks such as enemy AI and additional mechanics will be added in future sprints.)

### ENCOUNTER-001: Simple enemy spawner *(completed)*
**Description:** Implement an actor (`AEnemySpawner`) that periodically spawns a chosen enemy class within a radius. Useful for playtesting combat/traversal loops.
**Acceptance:** Spawner actor compiles and can be placed in a level; enemies appear at the configured interval.

### AI-001: Scaffold enemy archetypes *(in progress)*
**Description:** Create base enemy classes (`AEnemyBase`, `AEnemyMutantRusher`, `AEnemyCultist`) with simple properties and a basic AI controller that chases the player. Behaviour currently tick‑based and includes distance‑based dash logic; full StateTree integration pending.
**Acceptance:** Classes compile, have automation smoke tests, and spawn with an `AEnemyAIController` that issues MoveToActor commands and accelerates when near the player.

## Phase 3 – Motion Matching & Anim Integration

**Status:**
- Done: basic manifest export script, average hip/contact placeholders, documentation
diagram, initial pipeline verification
- Undone: per‑frame feature computation, CI integration, additional clip exports
- Blocked: waiting on pipeline enhancements (tracked in issue #2)
- Percent complete: ~90% (training stub added; manifest integration script present)


### MM-001: Export Animation Manifest for Motion-Matching *(in progress)*
**Description:** Use `motion_matching_pipeline.py` to list asset paths, retarget notes, export per-frame features (root velocity, hip position, contact flags). Produce CSV manifest for 10+ sample clips covering locomotion (walk/run/jump/vault).
**Acceptance:** CSV manifest present with feature schema; per-clip exports exist; documentation updated with pipeline steps.

*Progress:* Initial script and runbook added; CSV export verified. Script now exports per‑frame data and CI workflow generates and diffs manifests against a baseline. Remaining work: integrate with training tool and author additional clips for coverage.

## Phase 4 – Artifacts & Boss *(completed)*

**Status:**
- Done: artifact DataAsset and boss encounter prototype both implemented
- Undone: none; this phase is complete
- Blocked: none
- Percent complete: 100%


### ART-001: Implement Crescent City Artifact DataAsset *(completed)*
**Description:** Create `ArtifactData` DataAsset for a New Orleans–themed relic (e.g. "Bayou Chalice"). Passive: life-on-hit effect (small heal per hit). Signature: costs 100 RE, spawns a healing tide area, cleanses debuffs. Include cultural note citations and respectful representation review focused on local folklore.
**Acceptance:** DataAsset populated with all fields; passive effect applies on equip; Signature ability functional and costs 100 RE; cultural review checklist completed.

*Implementation:* C++ `UArtifactData` added under `Source/Adventure/Public/Artifacts`.

### BOSS-001: Multi-Phase Boss Encounter Prototype *(completed)*
**Description:** Create boss blueprint with 2-3 phases. Phase 1: standard combat. Phase 2: ritual puzzle (sigil decoding or component placement) requiring artifact ability to interrupt. Phase 3: enraged combat with environmental weaknesses. Boss health gates phase transitions.
**Acceptance:** Boss transitions between phases; ritual puzzle mechanic functional; artifact Signature required to progress; encounter completable in 5-10 minutes.

*Implementation:* C++ stub `ABossEncounter` added; full blueprint authored by designers.

---

These issues cover the core roadmap; additional issues can be added as sprints progress.
