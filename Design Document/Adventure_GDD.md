# Adventure
Game Design Document

## STAGE ONE: CONCEPT

### Game Summary
- One-line: Adventure is a hybrid action-adventure / supernatural horror in Old Nola, where Jessie “Nola” Savage explores occult mysteries, collects artifacts tied to the Seven African Powers, and uses Ritual Energy to perform powerful ritual abilities.
- Tone: Gritty, kinetic, and atmospheric; blend of exploration-driven discovery and tense, tactical combat.
- Platform & Prototype target: PC (UE5.7) — 3-month prototype, single-player (mobile TBD).

### Elevator Pitch
- Traverse Old Nola’s haunted streets, gather Ritual Energy through combat and investigation, and activate ritual artifacts to defeat occult bosses and unlock hidden lore.

### Main Character
Jessie (Nola) Savage — an occult scholar/adventurer driven by a personal quest; customizable look planned.

### Setting & Lore
Old Nola overlays ancient ritual geographies. Cabals, bayou crypts, and broken pacts form the narrative backbone. Lore is revealed through journals, ritual diagrams, NPCs, and environment storytelling.

### Player Perspective
Multi-perspective camera: Top-Down / Third-Person / First-Person — switchable via mouse wheel. Modes tuned for traversal, combat, and immersion respectively.

### Player Experience Goals
- Alternating tension (horror) and kinetic combat moments.
- Satisfying traversal and discovery (Tomb Raider-style exploration).
- Boss battles combining ritual puzzles and combat.
- Guns are useful but not always decisive — artifacts and rituals matter.

---

## STAGE TWO : SPECIFICATIONS

### Core Loop
Explore → Discover clues & resources → Encounter enemies → Fight/stealth/solve → Earn Ritual Energy → Spend Ritual Energy & Artifacts → Progress / Unlock

### Ritual Energy (tunable)
- Range: 0–100 (keep name `Ritual Energy`).
- Example gains: Light hit +3, Heavy/headshot +8, Stealth takedown +10, Solve major ritual +20, Parry +12.
- Passive decay: −1 / sec after 8s out of combat (configurable).
- Uses: Minor Surge (30), Ritual Flash (60), Signature Ritual (100).

### Combat
- Full GAS integration: abilities as `UGameplayAbility` with costs via `UGameplayEffect`.
- Hybrid combat: firearms (limited), melee (combos & parries), occult abilities, stealth.

### Exploration & Traversal
- Parkour-lite: vault, mantle, rope-swing, climb; boat traversal for bayou areas.

### Puzzles
- Voodoo sigil decoding, ritual component placement, chant/rhythm mechanics.

### Boss Battles
- Multi-phase with ritual puzzle phases and environmental weaknesses.

### UI / HUD
- Clean HUD with minimal/full toggles. Key elements: Health, Ritual Energy (prominent), Ammo, Stamina.

### Controls
- Mouse wheel toggles camera (TopDown → ThirdPerson → FirstPerson). Default bindings: LMB light, RMB heavy, Shift dodge, Q block, E interact, F ritual.

---

## Artifacts — The Seven (Gameplay & Lore)

### Overview
Jessie collects seven legendary artifacts tied to the Seven African Powers (Orishas). Each artifact grants passive and active bonuses, amplifies the Signature Ritual, and helps defeat specific boss mechanics.

> Cultural note: The Orishas are sacred figures. Represent them respectfully; include cultural review and credit sources.

### The Seven Artifacts & Effects
- **Elegua — Crossroads Talisman**: cooldown reduction; Signature offers a phase-choice (offense/defense/utility).
- **Yemaya — Oceanic Chalice**: life-on-hit passive; Signature spawns a healing tide and can cleanse certain curses.
- **Oshun — Mirror of Rivers**: charm/proc chance; Signature pacifies or converts swarms.
- **Shango — Thunderbrand**: increases damage/crit; Signature: lightning tempest.
- **Obatala — White Scepter**: defense/resilience; Signature: protective sanctum.
- **Oya — Stormblade**: movement/knockback and area control; Signature: localized storm.
- **Ogun — Ironforge Cleaver**: armor penetration; Signature: focused guard-breaking strike.

### Implementation Hooks
- Artifacts as DataAsset `ArtifactData` with fields: `Name`, `Orisha`, `BaseEffects`, `SignatureModifier`, `UpgradeTiers`.
- GAS: artifacts grant passive `GameplayEffects` and modify `GA_Ritual_Signature` (via tags or swapping effect specs).

---

## Motion-Matching Pipeline (high-level)
- Curate free animation packs, retarget to project skeleton, extract per-frame features (root vel, joint velocities, contact flags), build CSV manifest, index runtime DB, and integrate with AnimGraph. A montage fallback is acceptable while the database integration matures.

---

## Prototype Milestones (3-month plan)
- Sprint 1: UE5.7 setup, movement, camera toggle, basic HUD, `RitualEnergy` AttributeSet.
- Sprint 2: GAS base abilities, Ritual Energy flow, Grunt + Cultist.
- Sprint 3: Motion matching import & test; puzzle prototype; boss scaffold.
- Sprint 4: Boss finalization, VFX/SFX, mobile plan, playtest & tuning.

---

## Acceptance Criteria
- Camera toggle (TopDown/TP/FP) works via mouse wheel.
- GAS pipeline with `RitualEnergy` working and at least 2 Ritual abilities.
- Motion-matching dataset integrated or montage fallback operational.
- One multi-phase boss demonstrating ritual puzzle + combat.
