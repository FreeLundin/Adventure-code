"""
Guided refactor: Adventure* -> SVGLND_*
Run ONLY on staging branch.
Supports --dry-run.
"""

import os, re, argparse

parser = argparse.ArgumentParser()
parser.add_argument("--dry-run", action="store_true")
args = parser.parse_args()

root = "."
pattern = re.compile(r'\bAdventure([A-Za-z0-9_]*)\b')

for dp, _, files in os.walk(root):
    for f in files:
        if f.endswith((".h",".cpp",".py",".md")):
            path = os.path.join(dp,f)
            with open(path,"r",encoding="utf-8",errors="ignore") as fh:
                txt = fh.read()
            new = pattern.sub(r"SVGLND_\1", txt)
            if new != txt:
                print(f"[SVGLND] Update {path}")
                if not args.dry_run:
                    with open(path,"w",encoding="utf-8") as fh:
                        fh.write(new)
