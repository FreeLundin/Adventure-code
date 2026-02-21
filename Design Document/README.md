```markdown
# Documentation — Adventure (GameAnimationSample)

This folder contains design docs, runbooks, scripts, and the backlog for the
Adventure prototype. The goal is to make documentation easy to discover and
keep a concise status for contributors.

Start here
- `INDEX.md` — compact table of contents for documentation (recommended first read).
- `PROJECT_STATUS.md` — simplified plan and current progress against milestones.

Key docs (detailed)
- `Adventure_GDD.md` — full game design document.
- `Adventure_Implementation_Plan.md` — implementation breakdown and task templates.
- `Product_Backlog.md` and `issues/` — backlog items and issue templates.

Quick start
1. Open `INDEX.md` to find the doc or runbook you need.
2. To run editor scripts (examples in `scripts/`) open the Unreal
  Editor Python console and run the script entry point. Example:

```python
# In Unreal Editor Python console
import sys
sys.path.append(r"C:/Unreal_Projects/Adventure/Design Document")
from motion_matching_pipeline import collect_clips, build_manifest
clips = collect_clips('/Game/Animations')
build_manifest(clips, '/Saved/MotionMatching/Manifest.csv')
```

What we changed
- Added `INDEX.md` to simplify discovery.
- Added `PROJECT_STATUS.md` with a concise phase-by-phase status and next actions.
- Kept detailed documents intact (GDD, Implementation Plan, Roadmap) as source
  of truth for design and technical decisions.

How to contribute
- If you complete a TODO, update `PROJECT_STATUS.md` and convert the item into
  an issue under `issues/` with owner and estimate.

``` 