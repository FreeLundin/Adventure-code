import os, sys

FORBIDDEN_TOKENS = ["SoundCue", "Cascade", "UParticleSystem"]
ROOT_SCAN = ["Source", "Config", "Documentation"]

def scan():
    violations = []
    for root in ROOT_SCAN:
        if not os.path.isdir(root): 
            continue
        for dirpath, _, files in os.walk(root):
            for fn in files:
                if not any(fn.endswith(ext) for ext in [".h",".hpp",".cpp",".ini",".md",".py",".txt"]):
                    continue
                path = os.path.join(dirpath, fn)
                try:
                    with open(path, "r", encoding="utf-8", errors="ignore") as f:
                        for i,line in enumerate(f, start=1):
                            for tok in FORBIDDEN_TOKENS:
                                if tok in line:
                                    violations.append(f"{path}:{i} forbidden '{tok}'")
                except Exception as e:
                    violations.append(f"{path}:0 read error {e}")
    return violations

v = scan()
if v:
    print("❌ ARCHITECTURE CHECK FAILED\n" + "\n".join("- "+x for x in v))
    sys.exit(1)
print("✅ Architecture check passed.")
sys.exit(0)
