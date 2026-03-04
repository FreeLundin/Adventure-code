import subprocess, sys, fnmatch

PROTECTED_GLOBS = ["Plugins/ATS/**", "Plugins/ThirdParty/**", "Engine/**"]

def changed_files():
    r = subprocess.run(["git","diff","--name-only","origin/main...HEAD"], capture_output=True, text=True)
    return [l.strip().replace("\\","/") for l in r.stdout.splitlines() if l.strip()]

def is_protected(p):
    for pat in PROTECTED_GLOBS:
        if fnmatch.fnmatch(p, pat.replace("**","*")) or fnmatch.fnmatch(p, pat):
            return True
    return False

bad = [f for f in changed_files() if is_protected(f)]
if bad:
    print("❌ PROTECTED PATH VIOLATION:\n" + "\n".join(f"- {b}" for b in bad))
    sys.exit(1)
print("✅ Protected path check passed.")
sys.exit(0)
