#!/usr/bin/env python3
"""Execute the execution_checklist.yaml sequentially.
Usage: python Automation/run_checklist.py [--file PATH]
"""
import yaml, argparse, sys

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--file", default="Automation/execution_checklist.yaml")
    args = parser.parse_args()
    try:
        with open(args.file) as f:
            data = yaml.safe_load(f)
    except Exception as e:
        print(f"Failed to load checklist: {e}")
        sys.exit(1)

    for phase in data.get("phases", []):
        print(f"=== Phase {phase['id']} ({phase['name']}) status={phase['status']} ===")
        for step in phase.get("steps", []):
            print(f"- Step {step['id']}: {step.get('action')} {step.get('name','')}" )
            # no-op; actual execution would call relevant handlers
        for sub in phase.get("sub_tasks", []):
            print(f"  Subtask {sub.get('id')}: {sub.get('name')} (owner {sub.get('owner')})")
    print("Checklist run complete")

if __name__ == '__main__':
    main()
