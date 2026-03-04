> **NOTE:** Documentation is currently being consolidated; the canonical GDD now lives in `README.md`.  This issues list will be reviewed and realigned with the new roadmap.

# Actionable Issues from Simplified Roadmap

The following items correspond to the task templates defined in the implementation plan.  They should be created as GitHub issues (one per entry) and assigned to the appropriate owner or milestone.

## Phase 1 – Foundations

### CAM-001: Implement Multi-Perspective Camera Toggle
**Description:** Create camera component supporting TopDown/ThirdPerson/FirstPerson modes. Mouse wheel cycles through modes with smooth transitions. Each mode tuned for specific use: TopDown (traversal/puzzle), ThirdPerson (combat/exploration), FirstPerson (immersion/detail inspection).
**Acceptance:** Mouse wheel cycles camera modes; each mode has appropriate FOV and offset; transitions are smooth (<0.5s blend).

### RE-001: Implement Ritual Energy AttributeSet with Decay *(completed)*
**Description:** Create `AS_RitualEnergy` with float attribute (0-100 range). Implement passive decay (-1/sec after 8s out of combat). Expose to HUD with prominent bar visualization showing current value and decay state.
**Acceptance:** Ritual Energy decays correctly; combat prevents decay; HUD shows real-time updates; decay starts after 8s out of combat.

*Implementation added to `AAdventurePlayerState` tick and controller/HUD.*

### TODO-INPUT: Bind Enhanced Input & Controller Logic
**Description:** Resolve the TODOs in `PC_AdventureController.cpp` around enhanced input bindings, movement forwarding, camera rotation, sensitivity settings, and ability activation. These are foundational for playtesting.
**Acceptance:** PlayerController successfully binds actions, forwards input to the pawn, and camera input updates occur with sensitivity/inversion.

### TODO-REPLICATION: Add Replication Support to Character
**Description:** Address the `TODO` comments in `CBP_AdventureCharacter.h` regarding `GetLifetimeReplicatedProps`. Ensure all necessary properties are replicated for multiplayer.
**Acceptance:** Character properties replicate correctly in a two-client PIE session; no warnings about missing replication entries.

### TODO-CAMERA-SMOOTH: Smooth Camera Transitions
**Description:** Implement the smooth camera-transition logic teased in controller TODOs and update HUD to indicate current mode.
**Acceptance:** Switching camera modes yields a smooth blend; HUD reflects active mode.

## Phase 2 – Combat & GAS

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

## Phase 3 – Motion Matching & Anim Integration

### MM-001: Export Animation Manifest for Motion-Matching
**Description:** Use `motion_matching_pipeline.py` to list asset paths, retarget notes, export per-frame features (root velocity, hip position, contact flags). Produce CSV manifest for 10+ sample clips covering locomotion (walk/run/jump/vault).
**Acceptance:** CSV manifest present with feature schema; per-clip exports exist; documentation updated with pipeline steps.

## Phase 4 – Artifacts & Boss

### ART-001: Implement Crescent City Artifact DataAsset
**Description:** Create `ArtifactData` DataAsset for a New Orleans–themed relic (e.g. "Bayou Chalice"). Passive: life-on-hit effect (small heal per hit). Signature: costs 100 RE, spawns a healing tide area, cleanses debuffs. Include cultural note citations and respectful representation review focused on local folklore.
**Acceptance:** DataAsset populated with all fields; passive effect applies on equip; Signature ability functional and costs 100 RE; cultural review checklist completed.

### BOSS-001: Multi-Phase Boss Encounter Prototype
**Description:** Create boss blueprint with 2-3 phases. Phase 1: standard combat. Phase 2: ritual puzzle (sigil decoding or component placement) requiring artifact ability to interrupt. Phase 3: enraged combat with environmental weaknesses. Boss health gates phase transitions.
**Acceptance:** Boss transitions between phases; ritual puzzle mechanic functional; artifact Signature required to progress; encounter completable in 5-10 minutes.

---

These issues cover the core roadmap; additional issues can be added as sprints progress.
