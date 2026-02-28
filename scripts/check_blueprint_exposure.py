#!/usr/bin/env python3
"""Simple scanner to identify UPROPERTY declarations that lack Blueprint exposure.

Usage:
    python check_blueprint_exposure.py [path]

If no path is given, the workspace "Source/Adventure" directory is scanned.
"""
import os
import re
import sys

PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
SEARCH_ROOT = os.path.join(PROJECT_ROOT, "Source", "Adventure")

prop_pattern = re.compile(r"UPROPERTY\(([^)]*)\)")
blueprint_keywords = ["BlueprintRead", "BlueprintWrite", "BlueprintCallable", "BlueprintImplementableEvent", "BlueprintNativeEvent"]


def scan_file(path):
    with open(path, 'r', encoding='utf-8', errors='ignore') as f:
        for line_no, line in enumerate(f, 1):
            match = prop_pattern.search(line)
            if match:
                params = match.group(1)
                if not any(k in params for k in blueprint_keywords):
                    print(f"{path}:{line_no}: UPROPERTY missing Blueprint specifier -> {params}")


def main(root=SEARCH_ROOT):
    for dirpath, dirs, files in os.walk(root):
        for fname in files:
            if fname.endswith('.h') or fname.endswith('.cpp'):
                scan_file(os.path.join(dirpath, fname))


if __name__ == '__main__':
    target = sys.argv[1] if len(sys.argv) > 1 else SEARCH_ROOT
    main(target)
