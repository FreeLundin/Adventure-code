# Editor-time Python (run inside UE Editor)
import unreal, sys

ALLOWED_PREFIXES = ["NS_", "NE_", "FX_"]
violations = []

assets = unreal.EditorAssetLibrary.list_assets("/Game", recursive=True)
for p in assets:
    if not p.endswith(".NiagaraSystem"):
        continue
    a = unreal.EditorAssetLibrary.load_asset(p)
    name = a.get_name()
    if not any(name.startswith(x) for x in ALLOWED_PREFIXES):
        violations.append(f"{p}: naming must start with one of {ALLOWED_PREFIXES}")

if violations:
    unreal.log_error("NIAGARA PRESET ENFORCEMENT FAILED")
    for v in violations: unreal.log_error(v)
    sys.exit(1)

unreal.log("Niagara preset enforcement passed.")
sys.exit(0)
