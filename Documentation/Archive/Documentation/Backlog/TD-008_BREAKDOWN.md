# TD-008 — Motion Matching & Automation (Decomposed)

## TD-008-A — Per-frame Feature Extraction
- Artifact: SVGLND_MotionFeatures_*.csv
- Owner: IDE (Automation Test)
- Guardrails: No ATS edits, read-only pawn sampling

## TD-008-B — CI Feature Archival
- Artifact: svglnd-features-and-manifest.zip
- Owner: CI
- Guardrails: Archive only, no mutation

## TD-008-C — Motion Matching Sample Acquisition
- Artifact: Animation clips under Content/Animation/MotionMatching/
- Owner: Human (Animator)
- Guardrails: No automation edits to animation graphs

## TD-008-D — Motion Context Interfaces
- Artifact: ISVGLNDMotionContextProvider
- Owner: IDE (C++)
- Guardrails: Character/Anim layer only

## TD-008-E — Audit & Trend Reporting
- Artifact: weekly_audit_report.md
- Owner: CI
- Guardrails: Read-only aggregation
