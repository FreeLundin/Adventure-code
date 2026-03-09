# Vertical Slice Idea — Adventure

Goal
- Create a focused, playable vertical slice that demonstrates the core identity of Adventure: traversal, ritual-infused combat, and an artifact-driven encounter with narrative flavor rooted in Old Nola's occult mysteries.

Scope (1–2 week vertical slice)
- One small level (a derelict shotgun house + bayou backyard) with vertical traversal elements (porch, fence, low wall, tree branch, rope-swing)
- Player character Jessie "Nola" Savage with core movement (walk, sprint, jump, vault, mantle), camera toggle (TopDown/ThirdPerson/FirstPerson via mouse wheel), and basic HUD.
- Core abilities: `GA_LightAttack`, `GA_HeavyAttack`, `GA_Climb`/`GA_Vault` for traversal; `RitualEnergy` AttributeSet (0-100) present and displayed prominently.
- A single scripted encounter (mini-boss): a corrupted cultist using ritual attacks; the fight requires using an artifact ability (one of the Seven) to interrupt a ritual channel and win.

Key beats

- Opening: Jessie enters the house, explores, encounters clues (journal page with Orisha symbols) revealing ritual mechanics and lore about the Seven African Powers.
- Traversal demo: climb to roof via ladder/branch, cross a broken fence using vault/mantle, rope-swing across bayou gap.
- Ritual Energy flow: Combat with minor enemies builds Ritual Energy (+3 light, +8 heavy, +12 parry).
- Encounter: a mini-boss ritual channel — player must use traversal to reach elevation, activate artifact ability (Yemaya's Oceanic Chalice healing tide OR Shango's Thunderbrand lightning strike) to interrupt and finish the fight. Signature Ritual costs 100 Ritual Energy for decisive blow.

Technical requirements

- `AS_RitualEnergy` AttributeSet (float 0–100, decay -1/sec after 8s out of combat) with prominent HUD binding showing current value.
- GAS Abilities: `GA_LightAttack` (+3 Ritual Energy, costs 0), `GA_HeavyAttack` (+8 Ritual Energy), `GA_Parry` (+12 Ritual Energy), `GA_Climb`/`GA_Vault` (traversal movement with montages).
- `ArtifactData` DataAsset implementing one of the Seven (recommend Yemaya or Shango) with passive GameplayEffect + Signature ability costing 100 Ritual Energy.
- Simple enemy AI with Behavior Tree (states: Patrol -> Attack -> RitualChannel) and visible cast bar widget for ritual interruption mechanic.
- Camera system: mouse wheel toggle between TopDown -> ThirdPerson -> FirstPerson with smooth transitions.

Success criteria

- Playthrough time: 3–5 minutes, no blocking bugs.
- Player must use traversal + Ritual Energy management + artifact Signature ability to win (design enforces vertical approach and resource management).
- Camera toggle works smoothly via mouse wheel (TopDown/ThirdPerson/FirstPerson).
- HUD shows Health, RitualEnergy (prominent), Stamina, and artifact cooldown clearly.
- Ritual Energy gain/decay system is visible and affects combat decisions (players must build to 100 for Signature).
- Cultural representation: artifact and Orisha references are respectful and accurate.

Deliverables

- Level: `Maps/VS_House.umap` (Old Nola shotgun house + bayou backyard) with placed traversal props (ropes, vaults, climbs), scripted enemy spawner, and artifact pickup location.
- Core Systems: `AS_RitualEnergy` AttributeSet with decay logic, multi-perspective camera component (TopDown/TP/FP).
- GAS Abilities: `GA_LightAttack`, `GA_HeavyAttack`, `GA_Parry`, `GA_Climb`, `GA_Vault`, `GA_Ritual_Signature`.
- Artifacts: `ArtifactData` DataAsset (choose one: Yemaya or Shango) with passive effects and Signature modifier.
- AI: Simple cultist enemy blueprint with Behavior Tree (Patrol/Attack/RitualChannel states) and cast bar widget.
- HUD: Widget blueprint showing Health, RitualEnergy (prominent bar with decay visualization), Stamina, artifact cooldown.
- Docs: This file + dev runbook (`VS_Playtest_README.md`) for running the slice.

Runbook (quick)

1) Branch: `vertical-slice/house-v1`
2) Run Editor and open `Maps/VS_House`
3) Run the scaffold scripts to ensure abilities/attributes created (see `Docs/README.md`)
4) Play and iterate until success criteria met

Risks & mitigations

- Risk: missing animations for climb/attack/parry — mitigate with motion-matching montage fallback and temporary root motion or teleport for traversal.
- Risk: AI tuning time — keep the enemy simple with scripted Behavior Tree states to reduce iteration time; focus on visual clarity of ritual channel mechanic.
- Risk: Ritual Energy tuning balance — start with GDD values (+3/+8/+12), instrument logging, iterate based on playtest feedback.
- Risk: Cultural sensitivity with Orisha representation — include cultural review checklist and cite sources in code comments and documentation.
