# Runbook: Editor Python Scripts

Purpose: How to run editor Python scripts safely and where outputs are written.

Prerequisites
- Open the project in the Unreal Editor (UE5.7).
- Use the Editor Python console or the `Execute Python Script` command in the Editor.

Guidelines
- Keep `Scripts/` in the project and import via `sys.path.append(r"<project>/Scripts")`.
- Prefer running inside the Editor Python console to ensure `unreal` API availability.
- When a script writes a report, check `Scripts/usd_add_report.txt` or the path the script documents.
- Use `USD_PATH` env var to override sample USD paths when needed.

Safety
- Do not run scripts that make broad changes (bulk-delete assets) without review.
- Use source control: commit saved maps/levels before running scripts that change levels.

Example (Editor Python console):

```python
import sys
sys.path.append(r"C:\Unreal_Projects\Adventure\Design Document\scripts")
import run_add_usd_stage_actor_in_editor
run_add_usd_stage_actor_in_editor.run(r"C:\path\to\example.usda")
```

Report locations
- Most tools write to `Scripts/<tool>_report.txt` under the project directory.
