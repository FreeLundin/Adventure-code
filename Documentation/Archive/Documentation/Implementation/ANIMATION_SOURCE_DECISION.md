# Animation Source Identification Guide

**Date:** Feb 21, 2026  
**Status:** Decision document for Phase 1 animation selection  
**Impact:** CRITICAL PATH — animation choice blocks core movement implementation

---

## Context

Adventure requires animations for:
- **Core Movement:** Walk, Run, Sprint, Jump, Crouch, Idle
- **Traversal:** Vault, Mantle, Climb (with ledge interactions)
- **Combat:** Light Attack, Heavy Attack, Parry, Dodge, Knockback reactions
- **Interactions:** Door open, ladder climb, ritual animations
- **Upper Body:** Firearms aiming, grenade throws, ritual gestures
- **Reactions:** Hit reactions, stagger, knockdown, get-up, death

**Technical Requirements:**
- UE5.7 compatible
- Third-person humanoid rig
- Retargetable to project custom rig
- Supports motion matching (optional; montage fallback acceptable)
- Multiple animation slots for layering/blending

---

## Option 1: Advanced Locomotion System v4 (ALS v4)

**Link:** https://github.com/dyanikoglu/ALS-Refactored (free, community-maintained)

### Pros
✅ **Most Comprehensive** — 200+ production-quality animations  
✅ **Modern Design** — Built for animation blueprints & state machines, UE5-native  
✅ **Community Support** — Active Discord, ample tutorials  
✅ **Modular** — Pick-and-choose animations; not all-or-nothing  
✅ **Combat-Ready** — Includes parry, dodge, hit reactions  
✅ **Traversal Included** — Vault, mantle, climb stubs (need extension)  

### Cons
❌ **Large Import** — ~5GB+ project; can freeze editor during import  
❌ **Retarget Effort** — ~6-8 hours to retarget custom skeleton to ALS rig  
❌ **Montage Fallback Needed** — Some animations (rituals, boss interacts) not included; must create fallback  
❌ **Setup Complexity** — AnimBP + state machine setup takes 2-4 hours post-retarget  

### Timeline
- **Import & Project setup:** 1-2 hours (on lower-end hardware, could be 3-4)
- **Skeleton retarget:** 6-8 hours
- **AnimBP setup:** 2-3 hours
- **Testing & fixes:** 2-3 hours
- **Total: 11-20 hours** (potentially blocks Phase 1 if slipping)

### Recommendation
**Use ALS v4 IF you have 15-20 hours in this sprint and hardware can handle large import.**

---

## Option 2: Unreal Mannequin (UE5 Default)

**Included with engine**

### Pros
✅ **Zero Import Time** — Already in engine  
✅ **Fast Retarget** — 2-3 hours for a simple rig swap  
✅ **Low File Size** — Minimal editor bloat  
✅ **Familiar** — Every UE developer knows this skeleton  
✅ **Montage Fallback Works** — Mannequin has basic anims for fallback  

### Cons
❌ **Limited Animation Library** — ~50 anims; missing combat, traversal, interactions  
❌ **Generic Feel** — Locomotion feels stiff; not suited to kinetic movement expectation  
❌ **Ritual/Combat Missing** — Attack animations generic; not supernatural-feeling  
❌ **No Motion Matching** — Requires animation blueprint workarounds  
❌ **Poor Traversal** — Vault/mantle/climb have placeholder anims only  

### Timeline
- **Skeleton retarget:** 2-3 hours
- **Create montage fallback for missing anims:** 8-10 hours
- **AnimBP setup:** 1-2 hours
- **Testing & fixes:** 2 hours
- **Total: 13-17 hours** (fills Phase 1, but animations feel generic)

### Recommendation
**Use Mannequin ONLY IF time is critical and you're willing to accept generic feel for prototype.**

---

## Option 3: MetaHuman

**Link:** https://www.unrealengine.com/marketplace/en-US/metahuman  
**Cost:** Free (requires registration; Epic account linking)

### Pros
✅ **Professional Quality** — Highly polished, photorealistic
✅ **Rigged for UE5** — Zero retarget needed; ready to drop in  
✅ **Commercial Viability** — Production-ready models  
✅ **Facial Animation** — Includes face rig if needed for NPC interactions  

### Cons
❌ **Animation Library Separate** — MetaHuman provides MODEL only, not animations  
❌ **Requires ALS or Custom Anims** — Still need animation source post-import  
❌ **Setup Still 12+ Hours** — Import MetaHuman, then retarget ALS (if choosing ALS path)  
❌ **Overkill for Prototype** — Photorealism adds cost with no gameplay benefit yet  

### Timeline
- **Download & import MetaHuman:** 1-2 hours
- **Choose animation source** (e.g., ALS): +additional hours from Option 1 or 2  
- **Total: 12-22 hours** (same or worse than ALS alone)

### Recommendation
**Defer MetaHuman to post-Phase1.** Use Mannequin skeleton now, swap model later.

---

## Option 4: Minimal Custom Montage Fallback (Fast Track)

**No external dependency; leverage existing content**

### Approach
1. Use **Mannequin skeleton** (2-3 hours retarget)
2. Create **5-6 core movement montages** (Sprint, Jump, Dodge, Light Attack, Heavy Attack, Parry)
3. Reuse existing anims with **aiming offset layers** for directional movement
4. Create **placeholder animations** for traversal (vault = simple jump; mantle = crouch transition)

### Pros
✅ **Fastest Path** — 8-10 hours total  
✅ **Unblocks Level Testing** — Can test movement and combat loops  
✅ **Montage Fallback Pattern** — Established best practice for rapid prototyping  
✅ **Easy to Extend** — Can swap ALS in later without reworking code  

### Cons
❌ **Limited Feel** — Movement feels constrained to montage play  
❌ **No Blending** — Can't smoothly transition between animations  
❌ **Ritual Animations Weak** — Signature Ritual feels generic  
❌ **High Friction for Iteration** — Tweaking montage timing slower than full AnimBP  

### Timeline
- **Create 6 core montages:** 8-10 hours
- **AnimBP setup:** 2-3 hours
- **Total: 10-13 hours** (fastest option)

### Recommendation
**Best for rapid prototype.** Can iterate on gameplay feel before investing in full animation library.

---

## Decision Matrix

| Criteria | ALS v4 | Mannequin | MetaHuman | Custom Montage |
|----------|--------|-----------|-----------|-----------------|
| **Implementation Time** | 15-20h | 13-17h | 12-22h | 10-13h |
| **Animation Quality** | ⭐⭐⭐⭐⭐ | ⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ |
| **Combat Feel** | ⭐⭐⭐⭐⭐ | ⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ |
| **Traversal Support** | ⭐⭐⭐⭐ | ⭐ | ⭐⭐ | ⭐⭐ |
| **Motion Matching Ready** | Yes | No | No | No |
| **Blocks Phase 1?** | Minimal | No | Minimal | No |
| **Risk Level** | Low | Low | Medium | Low |

---

## Recommendation: Hybrid Approach (RECOMMENDED)

**Phase 1 (Immediate, Days 1-3):**
1. Use **Mannequin for skeleton retarget** (2-3 hours) — gets character in world
2. Create **core montages** for movement/combat (4-5 hours)
3. Build simple test level and verify GAS activation
4. **Total: 7-9 hours** — fast path to playable prototype

**Phase 2 (Post-vertical-slice, Days 4-7):**
1. Import **ALS v4 animations** (1-2 hours for import)
2. Extend project with full animation blueprint (6-8 hours)
3. Swap in Mannequin-to-ALS animation retarget
4. **Total: 7-10 hours** — upgrade to full suite

**Benefits of Hybrid:**
- ✅ Unblocks Phase 1 immediately (low risk)
- ✅ Lets gameplay drive animation choice (don't block on perfect anims)
- ✅ ALS becomes enhancement, not blocker
- ✅ Can test motion matching feasibility on real game feel
- ✅ Ritual/Boss-specific animations can be custom-created in parallel

---

## Action Items

### Immediate (Next 2 Hours)
- [ ] **Decision:** Approve hybrid approach (Mannequin → ALS path)
- [ ] **Alternative:** If rejecting hybrid, confirm choice: ALS now OR custom montages only

### If Hybrid Approved
- [ ] **Create Animation Retarget Plan** (1-hour doc)
  - Skeleton comparison: Mannequin vs. project custom rig
  - Retarget bone mapping
  - Testing checkpoints
- [ ] **Extract ALS v4 link & setup guide** (30 min)

### If Custom Montages Only
- [ ] **List 6-8 core montages** needed for Phase 1
- [ ] **Assign animation creators** if team-based

---

## References

- **ALS v4 GitHub:** https://github.com/dyanikoglu/ALS-Refactored
- **UE5 Animation Retargeting:** https://docs.unrealengine.com/5.0/en-US/retargeting-animations-in-unreal-engine/
- **GAS + Montages:** See `GAS_Implementation_Summary.md` section on ability activation
- **Motion Matching:** https://docs.unrealengine.com/5.0/en-US/motion-matching-in-unreal-engine/ (post-Phase1)

---

## Next Step

**Execute chosen path:**
1. **If Hybrid:** Proceed to Task 3 (Set DefaultPawn) while animation retarget plan is created
2. **If ALS:** Allocate 15-20 hours in sprint and begin import immediately
3. **If Custom:** Begin montage list and animator assignment
