# Savageland — Implementation Plan

---

## Metadata
- Project: Savageland
- Owner: Brian‑Christopher Lundin
- Engine: Unreal Engine 5.7
- Game Type: Single‑Player TPS Action‑Adventure (FPS optional)
- Core Systems: ATS (MD6), Modern RPG Template, GAS, StateTree
- Vertical Slice: VS‑000 “The Drowned Quarter”
- Status: Active
- Last Updated: 2026‑03‑01

---

## Purpose
Define a **step‑by‑step execution plan** to implement Savageland’s gameplay systems in a controlled, verifiable way, ensuring:
- Traversal remains the core survival mechanic
- Combat integrates cleanly without movement conflicts
- All systems support the Vertical Slice (VS‑000)

---

## Scope

### In Scope
- Player traversal via ATS
- Combat via Modern RPG Template
- Abilities and rules via GAS
- AI orchestration via StateTree
- Enemy archetypes and encounters
- One complete vertical slice

### Out of Scope
- Multiplayer
- Open‑world streaming
- End‑game progression systems
- Live‑ops or monetization systems

---

## Non‑Negotiable Constraints
- ATS is the sole authority for player locomotion and traversal
- Combat systems must not move the player
- GAS governs all actions, cooldowns, and tags
- StateTree governs high‑level state only
- No feature ships without acceptance criteria

---

## Milestones & Phases

### M0 — Foundation Lock
**Objective:** Stabilize project and lock architecture.

Tasks:
- Enable required plugins (GAS, StateTree)
- Validate ATS in isolation
- Validate Modern RPG Template in isolation
- Initialize GAS on PlayerState
- Align documentation and pitch deck

Exit Criteria:
- Clean PIE launch
- No traversal regressions
- Architecture frozen

---

### M1 — Traversal‑First Gameplay (ATS)
**Objective:** Prove traversal as survival mechanic.

Tasks:
- Integrate ATS into player character
- Validate climb, mantle, vault, swim
- Tune traversal camera offsets
- Stress‑test traversal under enemy pressure

Exit Criteria:
- Player can escape encounters using traversal
- No animation deadlocks
- Traversal readable in TPS

---

### M2 — Combat Integration
**Objective:** Add combat without breaking movement.

Tasks:
- Integrate Modern RPG Template combat
- Implement Combat Adapter Interface
- Implement ATS–Combat Coordinator
- Route all combat input through adapter
- Validate firearms and melee

Exit Criteria:
- Combat blocks during traversal
- Combat resumes immediately after traversal
- No duplicated movement logic

---

### M3 — Abilities & State Orchestration
**Objective:** Centralize rules and state.

Tasks:
- Implement GAS traversal abilities (GA_Climb, GA_Mantle, GA_Swim)
- Implement GAS combat abilities (Shoot, Melee, Cast)
- Add Ritual Energy AttributeSet
- Implement StateTree locomotion/combat states

Exit Criteria:
- All actions flow through GAS
- StateTree transitions stable and readable
- Tags reflect player state accurately

---

### M4 — Enemy Archetypes
**Objective:** Validate traversal‑combat loop with enemies.

Tasks:
- Implement AR‑001 Mutant Rusher
- Implement AR‑101 Cultist Hexer
- Implement AR‑102 Cultist Marksman
- Hook abilities and StateTree AI
- Validate enemy pressure behavior

Exit Criteria:
- Standing still is punished
- Traversal reduces pressure
- Priority targets are clear

---

### M5 — Vertical Slice (VS‑000)
**Objective:** Deliver a playable proof of the game.

Tasks:
- Build VS‑000 map blockout
- Place traversal routes
- Implement encounters per VS‑000 spec
- Add artifact reward
- Run full playthrough tests

Exit Criteria:
- 15–25 minute playable slice
- Start → escalation → payoff → exit
- All core systems exercised

---

### M6 — Polish & Submission
**Objective:** Prepare for academic/demo review.

Tasks:
- Bug fixing
- Camera polish
- Combat and AI tuning
- Performance pass
- Gameplay recording
- Final documentation

Exit Criteria:
- Stable build
- No blocking bugs
- Presentation‑ready materials

---

## Dependencies
- Advanced Traversal System (MD6)
- Modern RPG Template
- Gameplay Ability System
- StateTree
- Unreal Engine 5.7

## Tooling & Infrastructure
To support development and profiling we will integrate the following tools into the workflow:

- **Nsight Graphics/Compute/Systems** – GPU and system profilers for performance tuning. Install on all windows workstations and add to the engine's plugin list; capture traces nightly as part of CI smoke tests.
- **Microsoft 365** – Teams for chat/video, Planner for sprint tasks, OneDrive for shared documents, Power Automate for GitHub notifications. Establish project team and channel during M0.
- **Approach with official Unreal plugins only** – avoid third‑party/experimental plugins where possible. When a feature is needed (e.g. motion matching, USD support, animation retargeting) prefer the versions distributed and supported by Epic through the Marketplace or the engine source.  Official plugins often include better integration, up‑to‑date documentation, and long‑term support.

These tools are not required for the game logic itself, but will be standard across the studio and should be included in onboarding documentation.

> **Other useful tooling**
> * RenderDoc / NVIDIA Nsight Graphics for frame captures (alternative to Nsight extension).
> * PIX (Windows GPU/CPU capture) for low‑level debugging on Xbox/PC.
> * Visual Studio Profiler & concurrency visualizer for CPU hotspots.
> * GitHub Projects / GitHub CLI for issue/task management.
> * A good Diff/merge tool (WinMerge, BeyondCompare) for resolving large binary assets when LFS is not used.
> * Portable build farm scripts (PowerShell/azd) for consistent environment setup.

---

## Risks & Mitigations
- **Traversal/Combat Conflicts:** Early stress testing, strict ownership
- **Scope Creep:** Vertical Slice is hard cap
- **Camera Readability:** Separate tuning task, TPS priority

---

## Acceptance Criteria (Global)
- Traversal is required for survival
- Combat never owns movement
- Enemies force repositioning
- VS‑000 is playable end‑to‑end

---

## Status
✅ Phase 1 Complete — M2 Active (combat integration underway)
