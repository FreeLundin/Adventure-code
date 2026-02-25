```markdown
# VS Playtest — Run and Test

This document explains how to run and validate the Vertical Slice playtest locally.

Branch & Repo
- Branch naming: `feat/vertical-slice/{initials}` — replace `{initials}` with your handle.
- Create the branch and push: `git checkout -b feat/vertical-slice/<initials>` then commit/push.

Plugins to enable (project `*.uproject` Plugins list)
- Mover
- MoverExamples
- Network Prediction
- Enhanced Input
- NavigationSystem / AIModule
- Automation Testing / Functional Testing
- Editor Scripting Utilities
- Optional: Chaos, GameplayAbilities (GAS) if you plan physics-driven or ability-based features

Scaffold steps (local)
1. Copy example map as a scaffold:
   - Duplicate `Engine/Plugins/Experimental/MoverExamples/Content/Maps/L_CharacterMovementBasics.umap` into `Content/VS_Playtest/VS_Playtest.umap` (use the Editor to do this to preserve asset references).
2. Place `AnimatedMannyPawnExtended` (Mover-based pawn) in the map and set it as the Player Start pawn class.
3. Add `BP_SimpleMovingPlatform` and `BP_Zipline` from `MoverExamples/Content/Gameplay` into the map.
4. Create a minimal HUD widget `WB_VS_HUD` showing Health and a Rage placeholder and add it to the PlayerController BeginPlay.

Running the smoke test (Editor)
1. Open the project in UE5.7 Editor.
2. Enable required plugins (see list above) and restart the Editor if prompted.
3. Open `VS_Playtest.umap` and run Play-in-Editor (PIE) to verify manual behavior.

Automated smoke test (Editor Python)
Use the provided Editor Python script `Design Document/scripts/run_vs_smoke_test.py` to run a simple automated scenario.

Example command (run inside Unreal Editor Python console):

```python
import sys
import os
project_dir = os.environ.get("UE_PROJECT_DIR", r"C:/Unreal_Projects/Adventure")
sys.path.append(os.path.join(project_dir, "Design Document", "scripts"))
import run_vs_smoke_test as smoke
smoke.run()
```

Or run headless with UnrealEditor-Cmd (example):

```powershell
"C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor-Cmd.exe" "<path-to-project>\Adventure.uproject" VS_Playtest -run=pythonscript -script="Design Document/scripts/run_vs_smoke_test.py"
```

What to attach to the PR
- VS_Playtest.umap and added assets (under `Content/VS_Playtest/`)
- The automation script and captured logs (`Saved/Automation/VS-Smoke-*.log`)
- `Design Document/VS_Playtest_README.md` (this file)
- `Design Document/PROJECT_STATUS.md` entry and `Design Document/NOTIFY_TEAM.md` notification draft

Notes & Troubleshooting
- If assets are large, use Git LFS. Keep initial scaffold minimal (map with references to MoverExamples assets) to avoid large binary adds.
- If Editor scripts fail due to missing Editor context, run the snippet from the Editor Python console instead of headless.

``` 