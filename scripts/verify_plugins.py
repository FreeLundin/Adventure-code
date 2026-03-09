"""Simple check that only approved project plugins exist.
The approved list lives in Documentation/ApprovedPlugins.txt. Each line is a plugin folder name.
If the project contains any Plugins/* directories not on the whitelist, script exits with failure.
Usage: python scripts/verify_plugins.py
"""
import os
import sys

repo_root = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
plugins_dir = os.path.join(repo_root, "Plugins")
whitelist_file = os.path.join(repo_root, "Documentation", "ApprovedPlugins.txt")

approved = set()
if os.path.isfile(whitelist_file):
    with open(whitelist_file, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if line and not line.startswith("#"):
                approved.add(line)

if not os.path.isdir(plugins_dir):
    print("No project Plugins directory; nothing to verify.")
    sys.exit(0)

found = []
for entry in os.listdir(plugins_dir):
    path = os.path.join(plugins_dir, entry)
    if os.path.isdir(path):
        found.append(entry)

if not found:
    print("Plugin directory exists but contains no plugins.")
    sys.exit(0)

bad = [p for p in found if p not in approved]
if bad:
    print("Unapproved project plugins found:", bad)
    sys.exit(1)
else:
    print("All project plugins are approved or none present.")
    sys.exit(0)
