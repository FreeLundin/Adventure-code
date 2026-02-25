# ModelCard

**Name:**  
**Owner:**  @ml-lead

## Summary
Short description of model purpose and intended use.

## Provenance
- **Training dataset(s):** dataset-name vX.Y; manifest: `AI/datasets/<dataset>/manifest.json`
- **Training date:** YYYY-MM-DD
- **Training hardware:** e.g., 4x A100 80GB
- **Framework & versions:** e.g., PyTorch 2.2.0; CUDA 12.1
- **Code commit:** `<repo>@<commit-hash>`

## Reproducibility
- **Config file:** `AI/configs/<model>-train.yaml`
- **Random seed(s):** 42
- **Hyperparameters (high level):** lr=0.0001; batch=64; epochs=10
- **Exact training command:** `python train.py --config AI/configs/<model>-train.yaml --seed 42`

## Evaluation
- **Benchmark dataset:** `AI/benchmarks/<benchmark>.jsonl`
- **Metrics (baseline / current):** accuracy: 0.72 → 0.78; latency: 12ms
- **Evaluation command:** `python eval.py --model path/to/checkpoint --dataset AI/benchmarks/<benchmark>.jsonl`

## Inference
- **Expected latency (target HW):** 12ms @ RTX 4090
- **Memory footprint:** 2.1 GB
- **Fallback behavior:** deterministic state machine v1.0

## Risks & Limitations
Short list of known failure modes and mitigation.

## License & Third‑party
- **Model license:** Apache-2.0
- **Third-party components:** list and license

## Rollback & Canary
- **Canary rollout steps:** see `AI/runbooks/deploy_canary.md`
- **Rollback command:** `ai-deploy rollback --model <name> --to <stable-tag>`

## Contact
**Maintainer:** @ml-lead