"""
Pre-commit naming enforcement.
Fails if new symbols/assets lack SVGLND_ prefix.
"""

import sys, subprocess, re

BAD = []
pattern = re.compile(r'\b(Adventure[A-Za-z0-9_]*)\b')

diff = subprocess.check_output(
    ["git", "diff", "--cached", "--name-only"],
    text=True
).splitlines()

for f in diff:
    if f.endswith((".h", ".cpp", ".py", ".md")):
        content = subprocess.check_output(["git", "show", f":{f}"], text=True, errors="ignore")
        for m in pattern.findall(content):
            BAD.append(f"{f}: {m}")

if BAD:
    print("❌ SVGLND naming violation:")
    for b in BAD:
        print(" -", b)
    sys.exit(1)

print("✅ SVGLND naming check passed")