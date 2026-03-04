# Motion Matching Pipeline Runbook

This runbook describes how to export animation data for the motion-matching
system (Phase 3 deliverable).

## Overview

Motion matching relies on per-frame features extracted from animation clips.
Rather than hardcode these in blueprints, we maintain an external manifest that
lists every clip and the associated feature data.  The `motion_matching_pipeline.py`
script generates this manifest inside the Editor and is the canonical
starting point for any downstream tooling (Python, C#, etc.).

## Steps

1. **Open the project in Unreal Editor** (2025.7 or later required).
2. Navigate to `Window → Python Console`.
3. Execute the pipeline script:
   ```python
   exec(open(r"<project>/Automation/motion_matching_pipeline.py").read())
   ```
4. Monitor the Output Log for messages indicating how many assets were found
   and where the CSV was written (`Saved/AnimManifest.csv` by default).
5. Inspect the CSV to ensure the schema matches expectations.

## Customization

- To change the folder where locomotion clips live, modify `SEARCH_ROOT` in
  the script.  Typically clips are organized under
  `/Game/Animations/Locomotion/Walk`, `/Run`, `/Vault`, etc.
- Per‑frame extraction logic (root velocity, hip data, contact flags) is
  currently placeholder text; implement using Unreal's animation API when
  actual features are needed.

## Automation

- A future GitHub Actions job may call the script via the editor command-line
  and store the resulting manifest as a build artifact.  At present the
  pipeline remains manual; see TECH_DEBT item **TD-005**.

## Notes

- The CSV schema is intentionally flat: additional per-clip feature files can
  be added as separate columns or separate CSVs depending on tooling.
- The runbook should be updated whenever the pipeline expands (e.g. support for
  root motion metrics or tag metadata).

## References

- ISSUE **MM-001** in `ISSUES.md`
- TECH_DEBT **TD-005** for tracking automation status
