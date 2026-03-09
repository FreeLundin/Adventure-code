# SPEC: Next‑Gen Character Rebuild (CBP_AdventureCharacter)

**Project:** Savageland  
**Dev Codename:** Adventure  
**Owner (human):** Brian‑Christopher Lundin  
**Priority:** P0  
**Phase / Milestone:** Phase 2 – Combat & GAS (M2)  
**Status:** Proposed

---

## Goal

Create a **new, working `CBP_AdventureCharacter`** that modernizes traversal and locomotion by integrating **UE 5.7 Gameplay Framework features**—**Mover (Mover2)**, **StateTree**, and **Motion Matching**—while preserving the *design intent* of the Advanced Traversal System (ATS).

This character replaces the currently broken IDE‑generated CBP / GameMode / PlayerController setup and becomes the foundation for future traversal, combat, and animation work.

---

## Non‑Goals (Explicit)

- ❌ Do NOT modify ATS plugin source files  
- ❌ Do NOT remove existing ATS‑based character (keep as fallback)  
- ❌ Do NOT require multiplayer correctness (single‑player prototype)  
- ❌ Do NOT fully re‑author traversal animations  
- ❌ Do NOT replace GAS architecture (ASC remains PlayerState‑owned)

---

## Acceptance Criteria (Pass / Fail)

### Core Functionality
- [ ] Project boots into PIE with no errors or warnings
- [ ] Player is possessed correctly on BeginPlay
- [ ] Character can:
  - Move (forward / strafe / rotate)
  - Jump
  - Enter and exit traversal states cleanly
- [ ] No reliance on CharacterMovementComponent (Mover is active)

### Architecture
- [ ] `CBP_AdventureCharacter` inherits from a **C++ base pawn**
- [ ] MoverComponent is the **movement authority**
- [ ] StateTreeComponent orchestrates **high‑level locomotion state**
- [ ] Motion Matching drives **locomotion animation selection**
- [ ] ATS content is used **only as traversal data / animations**

### Stability
- [ ] No Tick‑driven traversal logic in Blueprints
- [ ] No broken references in GameMode or PlayerController
- [ ] Character can be swapped in GameMode without map edits

---

## Files / Assets (Expected)

### New
- `Source/Adventure/Public/AdventureMoverPawnBase.h`
- `Source/Adventure/Private/AdventureMoverPawnBase.cpp`
- `Content/Characters/CBP_AdventureCharacter.uasset`
- `Content/StateTrees/ST_PlayerLocomotion.uasset`
- `Content/Animation/PoseSearch/*` (Schema + Database)

### Modified / Replaced
- `BP_AdventureGameMode` (or C++ equivalent)
- `AdventurePlayerController`
- Map World Settings → GameMode Override

### Preserved
- Existing ATS‑based character Blueprint (unchanged)
- Existing traversal animations and assets

---

## High‑Level Design

### Movement Authority
**Mover (Experimental)**  
- Owns movement modes (grounded, falling, swimming, traversal)
- Handles layered movement (jump, impulse, root‑motion assists)

### Orchestration
**StateTree**
- Controls *what state the character is in*
- Example states:
  - Grounded
  - Airborne
  - Swimming
  - Traversal_Action
- Emits events to GAS / traversal adapters

### Animation
**Motion Matching (Pose Search)**
- Locomotion driven by Pose Search DB
- Traversal animations triggered as interactions / montages
- Debuggable via Rewind Debugger

### Gameplay Rules
**GAS**
- Remains PlayerState‑owned
- Traversal permissions & costs handled via GameplayAbilities
- Character queries ASC but does not own it

---

## Constraints

- Engine: Unreal Engine 5.7.3
- Plugins required:
  - Mover
  - StateTree
  - GameplayStateTree
  - PoseSearch
  - GameplayAbilities
- Must follow Blueprint Best Practices (event‑driven, readable graphs)
- All traversal extensions must be **additive**, not destructive

---

## Permissions (HITL)

- ✅ Assistant may generate SPECs
- ✅ Assistant may generate C++ scaffolding
- ✅ Assistant may generate Blueprint wiring diagrams
- ❌ Assistant may NOT commit code
- ❌ Assistant may NOT refactor ATS internals

---

## Test Plan (Minimum)

1. Open test map
2. Confirm correct GameMode is active
3. PIE:
   - Character spawns
   - Input responds
   - Locomotion animates via Motion Matching
   - Jump transitions cleanly
4. Trigger at least one traversal action
5. Stop PIE with no runtime errors

---

## Rollback Plan

- Revert GameMode DefaultPawnClass to ATS character
- Disable Mover / StateTree plugins
- Delete `CBP_AdventureCharacter` only (no ATS impact)

---

## Notes / Rationale

- Mover is experimental → this is an R&D‑forward but isolated implementation
- Two‑character strategy allows vertical slice to remain safe
- This SPEC becomes the reference for:
  - GA_Climb rewrite
  - Swimming overhaul
  - Artifact‑gated traversal
  - Motion‑matching expansion

---

## Success Definition

When this SPEC is complete:
> “Traversal feels modern, animation feels responsive, and the character architecture no longer fights Unreal 5.7’s direction.”
