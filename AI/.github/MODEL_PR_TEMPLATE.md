# Model PR Template

**Summary**
- Short summary of change:

**ModelCard**
- Link to `AI/templates/ModelCard.md` or existing ModelCard: `AI/models/<model>/ModelCard.md`

**Training config**
- Config file(s): `AI/configs/<model>-train.yaml`

**Benchmark & Metrics**
- Baseline metrics (before): 
- New metrics (after):
- Pass/fail thresholds:

**Repro steps**
- How to reproduce training and evaluation (one-line commands):

**Resource estimate**
- GPU hours, disk, memory:

**Rollback plan**
- Canary percentage, monitoring metrics, rollback command:

**Files changed**
- List of files (models, configs, scripts)

**CI**
- Ensure model CI passes (dataset validation, smoke inference)

**Owner**
- @ml-lead