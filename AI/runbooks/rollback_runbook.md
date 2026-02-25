# Rollback Runbook

**Purpose:** one-line, low-friction rollback steps for on-call engineers to revert a model to the last stable version.

## Preconditions
- You have MCP API token in environment: `export API_TOKEN="..."`.
- You know the model name and the stable version tag (e.g., `v0-stable`).

## Quick rollback (one-line)
```bash
# Replace MODEL and STABLE_VERSION and MCP_BASE_URL as needed
export MODEL="example-model"
export STABLE_VERSION="v0-stable"
export MCP_BASE_URL="https://mcp.example.com/api"
export API_TOKEN="REPLACE_ME"
curl -s -X POST "${MCP_BASE_URL}/models/rollback" \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer ${API_TOKEN}" \
  -d "{\"name\":\"${MODEL}\",\"to_version\":\"${STABLE_VERSION}\"}" | jq .
```