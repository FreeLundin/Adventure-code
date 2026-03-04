"""
SVGLND — Write Run Audit

Creates a JSON artifact after each unattended/supervised run.
Designed to be called by CI (GitHub Actions / Azure DevOps) or locally.

Output:
  Automation/audits/<run_id>.json
"""

import os
import json
import uuid
from datetime import datetime, timezone
import subprocess

AUDIT_DIR = os.path.join("Automation", "audits")
os.makedirs(AUDIT_DIR, exist_ok=True)

def git(cmd):
    try:
        return subprocess.check_output(["git"] + cmd, text=True).strip()
    except Exception:
        return ""

def main():
    run_id = os.environ.get("SVGLND_RUN_ID", str(uuid.uuid4()))
    mode = os.environ.get("SVGLND_MODE", "supervised_autonomy")

    audit = {
        "project_code": "SVGLND",
        "run_id": run_id,
        "timestamp_utc": datetime.now(timezone.utc).isoformat(),
        "mode": mode,
        "git": {
            "branch": git(["rev-parse", "--abbrev-ref", "HEAD"]),
            "commit_before": os.environ.get("SVGLND_COMMIT_BEFORE", git(["rev-parse", "HEAD"])),
            "commit_after": os.environ.get("SVGLND_COMMIT_AFTER", git(["rev-parse", "HEAD"]))
        },
        "guards": {
            "protected_paths_check": os.environ.get("SVGLND_GUARD_PROTECTED", "skipped"),
            "architecture_check": os.environ.get("SVGLND_GUARD_ARCH", "skipped"),
            "niagara_preset_enforcement": os.environ.get("SVGLND_GUARD_NIAGARA", "skipped")
        },
        "build": {
            "attempted": os.environ.get("SVGLND_BUILD_ATTEMPTED", "false").lower() == "true",
            "result": os.environ.get("SVGLND_BUILD_RESULT", "skipped"),
            "log_path": os.environ.get("SVGLND_BUILD_LOG", "")
        },
        "tests": {
            "attempted": os.environ.get("SVGLND_TESTS_ATTEMPTED", "false").lower() == "true",
            "result": os.environ.get("SVGLND_TESTS_RESULT", "skipped"),
            "ran": [t for t in os.environ.get("SVGLND_TESTS_RAN", "").split(";") if t],
            "failed": [t for t in os.environ.get("SVGLND_TESTS_FAILED", "").split(";") if t]
        },
        "tasks": []
    }

    # Optional: ingest task results from env var as JSON array
    raw_tasks = os.environ.get("SVGLND_TASK_RESULTS_JSON", "")
    if raw_tasks:
        try:
            audit["tasks"] = json.loads(raw_tasks)
        except Exception:
            audit["tasks"] = [{"id":"UNKNOWN","title":"Could not parse SVGLND_TASK_RESULTS_JSON","status":"failed"}]

    out_path = os.path.join(AUDIT_DIR, f"{run_id}.json")
    with open(out_path, "w", encoding="utf-8") as f:
        json.dump(audit, f, indent=2)

    print(f"[SVGLND] Wrote audit: {out_path}")

if __name__ == "__main__":
    main()
