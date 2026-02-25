# Change Tracking Workflow

**Purpose**: Ensure ALL changes to Adventure are documented for non-technical stakeholders and future maintainers.

## The Problem We're Solving

As a non-technical project owner, you need to:
- Understand what changed and why
- Track progress toward milestones
- Identify risks early
- Make informed decisions

When changes aren't logged, you lose visibility and the project becomes a "black box."

---

## The Solution: Mandatory Change Log Updates

### Rule: No Code Commit Without CHANGELOG Update

Every session that modifies code, config, or assets MUST update `CHANGELOG.md` with:

1. **What** changed (specific files, features, settings)
2. **Why** it changed (problem being solved, feature being added)
3. **Impact** (what this enables, what risks it introduces)
4. **How to verify** it works
5. **How to roll back** if needed

---

## Quick Start Guide (For AI Assistants)

### At Session Start:
```markdown
1. Add new section to CHANGELOG.md with today's date
2. List planned work items
3. Mark as [PLANNED]
```

### During Session:
```markdown
1. Update CHANGELOG section as work progresses
2. Mark completed items as [COMPLETE]
3. Document any deviations from plan
```

### At Session End:
```markdown
1. Finalize CHANGELOG entry with:
   - All files added/modified/deleted
   - Build status (time, warnings, errors)
   - Testing performed
   - Known issues or limitations
2. Update PROJECT_STATUS.md with milestone progress
3. Create SESSION_SUMMARY.md for major milestones
```

---

## Change Categories

### 🔵 Code Changes
- New classes/functions added
- Existing code modified
- Code deleted or refactored
- Public API changes
- Dependencies added/removed

**Required Info**:
- File paths
- Line count added/removed
- Why this code is needed
- What systems it affects

### ⚙️ Configuration Changes
- .ini file modifications
- Project settings
- Editor preferences
- Build settings
- Plugin enables/disables

**Required Info**:
- Exact setting name and value
- Why default wasn't sufficient
- How to verify it works
- Rollback procedure

### 📦 Asset Changes  
- Blueprints created/modified
- Content migrated
- Materials/textures added
- Level changes

**Required Info**:
- Asset paths
- Source (created new vs migrated)
- Integration points with code
- Reparenting requirements

### 📝 Documentation Changes
- Design docs updated
- New guides created
- Roadmap changes
- Architecture decisions

**Required Info**:
- What decision was made
- Why alternatives were rejected
- Impact on future work

---

## Examples

### ✅ GOOD Changelog Entry
```markdown
## [2026-02-24] - Animation Notify System

### Added
- UAnimNotify_FoleyEvent (Source/Adventure/Public/Animation/AnimNotify_FoleyEvent.h|cpp)
  - **Why**: Needed for footstep audio triggering in animation sequences
  - **Impact**: Audio team can now place foley events in animations
  - **Verification**: Play any animation with notify, look for UE_LOG output
  - **Dependencies**: Requires audio assets in Content/Audio/Foley/
  - **Lines**: 130 lines (65 header, 65 impl)

### Configuration
- DefaultEngine.ini: Enabled NewGameplayCameraSystem
  - **Setting**: DDCVar.NewGameplayCameraSystem.Enable=True
  - **Why**: Required for UE5.7 gameplay camera features
  - **Impact**: Camera system uses newer rendering path
  - **Verification**: PIE and observe camera smoothness
  - **Rollback**: Set to False if camera issues occur
```

### ❌ BAD Changelog Entry
```markdown
## [2026-02-24] - Updates

### Changed
- Fixed stuff
- Added things
- Modified files
```
**Problems**: No file paths, no reasoning, no verification, no rollback

---

## Configuration Change Checklist

Before changing ANY .ini file setting:

- [ ] Document current value
- [ ] Document new value
- [ ] Explain why default isn't sufficient
- [ ] List which systems are affected
- [ ] Describe how to verify it works
- [ ] Describe how to roll back
- [ ] Add entry to CHANGELOG.md
- [ ] Test in clean editor launch

---

## When Changes Break Things

If a change causes issues:

1. **Document the failure** in CHANGELOG
2. **Attempt fix** or **rollback**
3. **Update CHANGELOG** with resolution
4. **Add to Troubleshooting** section in relevant runbook

Example:
```markdown
### Fixed
- Camera system crash on PIE
  - **Issue**: Access violation in camera tick
  - **Root Cause**: GameplayCamera component not initialized before use
  - **Solution**: Added null checks in BeginPlay
  - **Files**: CBP_AdventureCharacter.cpp line 234
  - **Verification**: PIE launches without crash
  - **Lesson Learned**: Always null-check TObjectPtr components
```

---

## Integration with Existing Docs

### CHANGELOG.md (This File)
- **Granular changes** - every code/config/asset change
- **Chronological** - newest first
- **Technical details** - file paths, settings, line counts

### PROJECT_STATUS.md  
- **High-level status** - milestone progress
- **Actionable** - what's blocking, what's next
- **Brief** - one-line summaries

### SESSION_FINAL_SUMMARY.md
- **Major milestones** - end of phase, major feature complete
- **Deliverables** - what's ready to use
- **Metrics** - LOC added, build times, asset counts

### Governance.md
- **Process rules** - how to track changes (this workflow)
- **Templates** - issue/PR templates
- **Ownership** - who approves what

---

## AI Assistant Checklist

Before ending ANY session:

- [ ] CHANGELOG.md updated with ALL changes
- [ ] PROJECT_STATUS.md updated if milestone affected
- [ ] Build succeeded and time recorded
- [ ] All new files documented with paths
- [ ] All config changes documented with values
- [ ] Impact section filled out
- [ ] Verification steps provided
- [ ] Rollback procedure documented (if applicable)

---

## Benefits of This Process

### For Non-Technical Stakeholders:
- ✅ See exactly what's being built
- ✅ Understand why decisions were made
- ✅ Track progress toward milestones
- ✅ Identify risks early

### For Development:
- ✅ Future AI assistants understand past decisions
- ✅ Onboarding new team members is faster
- ✅ Debugging is easier (can bisect changes)
- ✅ Technical debt is visible

### For Project Success:
- ✅ No "mystery changes" that cause confusion
- ✅ Clear audit trail for all work
- ✅ Easier to roll back failed experiments
- ✅ Better estimates (can measure velocity)

---

## Enforcement

1. **Human Review**: Before accepting work, verify CHANGELOG was updated
2. **Build Script**: Consider pre-commit hook that checks for CHANGELOG updates
3. **Session Reviews**: End each session by reviewing CHANGELOG together

---

## Template for Next Session

```markdown
## [2026-MM-DD] - [Brief Session Description]

### Planned Work
- [ ] Task 1 description
- [ ] Task 2 description

### Added
- [File/Feature]: 
  - **Why**: 
  - **Impact**: 
  - **Verification**: 
  - **Lines**: 

### Changed
- [File/Setting]:
  - **Why**:
  - **Impact**:

### Configuration
- [Config File]: [Setting Name] = [Value]
  - **Why**:
  - **Impact**:
  - **Verification**:
  - **Rollback**:

### Fixed
- [Issue Description]:
  - **Root Cause**:
  - **Solution**:
  - **Verification**:

### Build Status
- Compilation: SUCCESS/FAILED
- Time: XX.XX seconds
- Warnings: X
- Errors: X

### Impact
- Positive: 
- Negative/Risk:
- Neutral:

### Next Session
- [ ] Priority 1 task
- [ ] Priority 2 task
```

---

Remember: **Every change tells a story. Make sure that story is written down.**
