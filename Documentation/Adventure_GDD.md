> **ARCHIVED DOCUMENT**  
> This file contains the original Game Design Document prior to 2026‑03‑03.  
> The authoritative GDD has been merged into `README.md` (repository root).  
> Refer to the README for the current design and roadmap.

# Adventure
Game Design Document

## 1. Game Summary
Adventure is a supernatural action-adventure set in Old Nola, where Jessie “Nola” Savage uncovers occult mysteries, battles corrupted entities, and channels Ritual Energy to activate powerful abilities tied to the Seven African Powers.

Tone blends gritty atmosphere, kinetic traversal, and ritual-driven combat.

## 2. Elevator Pitch
Explore haunted districts of Old Nola, gather Ritual Energy through combat and investigation, and activate ancient artifacts to defeat occult bosses and unlock hidden lore.

## 3. Main Character
Jessie “Nola” Savage — an occult scholar-adventurer with deep ritual knowledge and a personal stake in the supernatural events unfolding across Old Nola.

## 4. Setting & Lore
Old Nola overlays ancient ritual geographies. Cabals, bayou crypts, and broken pacts shape the world. Lore is delivered through journals, ritual diagrams, NPC encounters, and environmental storytelling.

## 5. Player Perspective
A multi-perspective camera system:

Top-Down — tactical exploration

Third-Person — traversal and combat

First-Person — immersion and investigation

Mouse wheel cycles modes.

## 6. Player Experience Goals
Rhythms of tension to release (horror to combat)

Satisfying traversal and discovery

Boss battles mixing ritual puzzles with action

Firearms useful but rituals define mastery

## 7. Core Gameplay Loop
Explore → Discover → Encounter → Fight/Stealth/Solve → Earn Ritual Energy → Spend on Ritual Abilities and Artifacts → Progress

## 8. Ritual Energy System
Properties
Range: 0–100

Passive decay: -1/sec after 8 seconds out of combat

Stored in a dedicated AttributeSet

Gains
Light hit: +3

Heavy/headshot: +8

Stealth takedown: +10

Major ritual solved: +20

Parry: +12

Uses
Minor Surge (30)

Ritual Flash (60)

Signature Ritual (100)

## 9. Combat System
Hybrid of melee, firearms, stealth, and ritual abilities.

Technical Foundation
Fully implemented using GAS

Abilities as UGameplayAbility

Costs/cooldowns via GameplayEffect

Motion-warping melee

Player Actions
Light Attack

Heavy Attack

Parry

Dodge

Signature Ritual

Firearms (limited ammo)

Stealth takedowns

## 10. Exploration & Traversal
Vault

Mantle

Climb

Rope swing

Bayou boat traversal

Traversal interacts with stamina and sometimes Ritual Energy.

## 11. Puzzle Design
Voodoo sigil decoding

Ritual component placement

Chant/rhythm timing

Environmental ritual circuits

## 12. Boss Battles
Multi-phase encounters combining:

Ritual puzzle phases

Environmental hazards

Combat phases with artifact weaknesses

Interrupt windows requiring parry or Ritual Flash

## 13. UI / HUD
Minimalist HUD:

Health

Ritual Energy

Stamina

Ammo

Artifact indicator

## 14. Controls
LMB: Light attack

RMB: Heavy attack

Shift: Dodge

Q: Block/Parry

E: Interact

F: Ritual ability

Mouse Wheel: Camera mode toggle

## 15. Artifacts — The Seven
Artifacts tied to the Seven African Powers (Orishas).
Each grants passive bonuses, active abilities, and modifies the Signature Ritual.

Cultural note: Represent Orishas respectfully; consult cultural experts.

Artifact List
Elegua — Crossroads Talisman: cooldown reduction; flexible Signature modes

Yemaya — Oceanic Chalice: life-on-hit; healing tide

Oshun — Mirror of Rivers: charm/pacify; swarm control

Shango — Thunderbrand: crit/damage; lightning tempest

Obatala — White Scepter: defense; protective sanctum

Oya — Stormblade: mobility/knockback; localized storm

Ogun — Ironforge Cleaver: armor break; guard-shattering strike

Implementation
Artifacts implemented as DataAssets with:

Name

Orisha

BaseEffects

SignatureModifier

UpgradeTiers

## 16. Enemies (Required by Coursera Assignment)
(Aligned with the checklist in your active tab)

### 16.1 Enemy Types
Grunt — “Hollowed”
Appearance: Emaciated humanoids with ritual scars and flickering ember-eyes.

Behavior: Swarm in groups, rush the player, low health.

Attacks: Claw swipe (short range), lunge (medium range).

Role: Early-game pressure and Ritual Energy farm.

Cultist — “Binder”
Appearance: Hooded figures with bone charms and ritual masks.

Behavior: Mid-range casters; maintain distance.

Attacks: Hex bolts, ritual snares, stagger curses.

Role: Forces player to close distance or use cover.

Bayou Beast — “Mireborn”
Appearance: Large, moss-covered creature with glowing runes.

Behavior: Slow but tanky; area denial.

Attacks: Ground slam, poison spit, swamp pull.

Role: Environmental hazard plus mini-boss.

Boss — “The Broken Loa”
Appearance: Massive spectral entity bound by corrupted sigils.

Behavior: Multi-phase; alternates between ritual puzzle and combat.

Attacks: Soul lash, ritual storms, summon adds.

Role: Capstone encounter demonstrating all systems.

## 17. Enemy Combat Mechanics
Attack Ranges:

Hollowed: 1–3m

Binder: 8–12m

Mireborn: 2–6m

Boss: arena-wide abilities

Damage Tuning:

Hollowed: low

Binder: medium burst

Mireborn: high, slow

Boss: variable per phase

Behavior Systems:

Aggro radius

Flanking logic

Ritual channeling (Binders/Boss)

Interrupt windows (parry or Ritual Flash)

## 18. Levels (Required by Coursera Assignment)
(Matches the assignment’s “Enemies and Levels” section)

### 18.1 Level 1 — Old Nola Streets
Theme: Abandoned French-Creole district under ritual corruption.

Flow:

Start at safehouse

Explore alleyways

Encounter Hollowed swarm

Solve sigil door puzzle

Mini-boss: Mireborn

Rewards:

Elegua Fragment

Ritual Energy caches

Journal pages

### 18.2 Level 2 — Bayou Crypt
Theme: Flooded ritual catacombs.

Flow:

Boat traversal

Stealth section with Binders

Multi-room ritual circuit puzzle

Boss arena prep

Rewards:

Artifact upgrade materials

Yemaya Fragment

Rare Ritual Components

### 18.3 Level 3 — The Broken Sanctum (Boss Level)
Theme: Collapsing ritual nexus.

Flow:

Environmental hazards

Ritual puzzle phase

Combat phase

Signature Ritual finale

Rewards:

Full Signature Ritual unlock

Major lore reveal

## 19. Illustrative Examples
(Required by assignment)

Example 1 — Combat Loop:
Player parries a Hollowed, gains Ritual Energy, then uses Ritual Flash to stagger a Binder before finishing with a Signature Ritual.

Example 2 — Level Flow:
In the Bayou Crypt, the player solves a sigil circuit puzzle to drain water, opening a path to the boss arena.

Example 3 — Artifact Synergy:
With Shango’s Thunderbrand equipped, heavy attacks generate lightning arcs that chain between clustered enemies.

## 20. Design Justification
(Required by assignment)

Audience
Fans of action-adventure games with supernatural themes (e.g., God of War, Control, Tomb Raider).

Market Fit
The ritual-energy combat loop and artifact system differentiate the game from typical melee-focused action titles.

Technical Constraints
UE5.7 with GAS ensures scalable combat.

Motion matching is optional; montage fallback ensures prototype viability.

Multi-perspective camera supports accessibility and player preference.

## 21. Acceptance Criteria
Camera toggle functional

RitualEnergy AttributeSet working

At least two Ritual abilities implemented

Motion-matching or montage fallback

One multi-phase boss with ritual puzzle and combat
