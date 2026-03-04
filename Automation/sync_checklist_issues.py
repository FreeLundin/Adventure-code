#!/usr/bin/env python3
"""Synchronize execution checklist phases/steps with GitHub issues.
Requires GITHUB_TOKEN env var and repo from git remote.
"""
import os, yaml, requests, subprocess, re

GITHUB_API = "https://api.github.com"

def get_repo():
    out = subprocess.check_output(["git", "config", "--get", "remote.origin.url"], text=True)
    # parse owner/repo from url
    m = re.search(r"[:/](.+?/.+?)(?:\.git)?$", out.strip())
    return m.group(1) if m else None


def upsert_issue(repo, token, title, body):
    headers = {"Authorization": f"token {token}"}
    # search existing
    q = requests.get(f"{GITHUB_API}/search/issues", headers=headers,
                     params={"q": title + " repo:"+repo})
    if q.ok and q.json().get("total_count",0)>0:
        issue = q.json()["items"][0]
        # update
        requests.patch(issue["url"], headers=headers, json={"body": body})
    else:
        requests.post(f"{GITHUB_API}/repos/{repo}/issues", headers=headers,
                      json={"title": title, "body": body})


def main():
    token = os.getenv("GITHUB_TOKEN")
    if not token:
        print("GITHUB_TOKEN required")
        return
    repo = get_repo()
    if not repo:
        print("unable to determine repo")
        return
    data = yaml.safe_load(open("Automation/execution_checklist.yaml"))
    for phase in data.get("phases", []):
        title = f"[{phase['id']}] {phase['name']}"
        body = f"Status: {phase['status']}\nDepends on: {phase.get('depends_on')}\nSteps:\n"
        for step in phase.get('steps',[]):
            body += f"- {step['id']} {step.get('action')} {step.get('name','')}\n"
        upsert_issue(repo, token, title, body)
    print("Sync complete")

if __name__=='__main__':
    main()
