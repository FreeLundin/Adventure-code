"""
Aggregates write_run_audit.py outputs into weekly summaries.
"""

import os, json, datetime
from collections import Counter

AUDITS = "Automation/audits"
now = datetime.datetime.utcnow()
week = now.strftime("%Y-W%U")

guards = Counter()
features = 0

for fn in os.listdir(AUDITS):
    if fn.endswith(".json"):
        with open(os.path.join(AUDITS,fn)) as f:
            a = json.load(f)
        for k,v in a.get("guards",{}).items():
            guards[f"{k}:{v}"] += 1

report = [
  f"# SVGLND Weekly Audit Summary ({week})",
  "",
  "## Guardrail Outcomes"
]

for k,v in guards.items():
    report.append(f"- {k}: {v}")

out = f"Automation/reports/weekly_audit_{week}.md"
os.makedirs("Automation/reports", exist_ok=True)
with open(out,"w") as f:
    f.write("\n".join(report))

print(f"[SVGLND] Wrote {out}")