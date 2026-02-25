#!/usr/bin/env bash
# canary_control.sh
# Simple MCP-driven canary automation for AAA studio workflows.
# Requirements: curl, jq
# Configure: export MCP_BASE_URL="https://mcp.example.com/api" ; export API_TOKEN="token_here"

set -euo pipefail

MCP_BASE_URL="${MCP_BASE_URL:-https://mcp.example.com/api}"
API_TOKEN="${API_TOKEN:-}"
MODEL_NAME="${1:-example-model}"
MODEL_VERSION="${2:-v1.0}"
CANARY_PERCENT="${3:-10}"   # percent of agents to route to canary
METRIC_WINDOW_SECONDS="${4:-300}" # window to evaluate metrics
LATENCY_THRESHOLD_MS="${5:-50}"   # example threshold
ERROR_RATE_THRESHOLD="${6:-0.02}" # example threshold (2%)

auth_header() {
  if [ -n "$API_TOKEN" ]; then
    echo "Authorization: Bearer $API_TOKEN"
  fi
}

log() { echo "[$(date -u +'%Y-%m-%dT%H:%M:%SZ')] $*"; }

promote_canary() {
  log "Promoting model ${MODEL_NAME}@${MODEL_VERSION} to canary ${CANARY_PERCENT}%"
  payload=$(jq -n --arg name "$MODEL_NAME" --arg version "$MODEL_VERSION" --arg target "canary" --argjson percent "$CANARY_PERCENT" '{name:$name,version:$version,target:$target,canary_percent:$percent}')
  curl -s -X POST "${MCP_BASE_URL}/models/promote" \
    -H "Content-Type: application/json" \
    -H "$(auth_header)" \
    -d "$payload" | jq .
}

fetch_metrics() {
  # Query MCP for aggregated metrics for the model (assumes MCP exposes such endpoint)
  log "Fetching metrics for ${MODEL_NAME}@${MODEL_VERSION}"
  resp=$(curl -s -G "${MCP_BASE_URL}/models/metrics" \
    --data-urlencode "model_name=${MODEL_NAME}" \
    --data-urlencode "model_version=${MODEL_VERSION}" \
    -H "$(auth_header)")
  echo "$resp"
}

evaluate_metrics_and_maybe_rollback() {
  metrics_json="$1"
  # Extract simple metrics (latency_ms, error_rate) - adapt to your MCP schema
  latency=$(echo "$metrics_json" | jq -r '.metrics.latency_ms // empty')
  error_rate=$(echo "$metrics_json" | jq -r '.metrics.error_rate // empty')

  log "Observed latency: ${latency} ms; error_rate: ${error_rate}"
  # If metrics missing, be conservative and trigger rollback
  if [ -z "$latency" ] || [ -z "$error_rate" ]; then
    log "Metrics missing or incomplete; triggering rollback"
    rollback_to_stable
    return
  fi

  # Compare thresholds (float comparison)
  latency_exceeded=$(awk -v a="$latency" -v b="$LATENCY_THRESHOLD_MS" 'BEGIN{print (a > b)}')
  error_exceeded=$(awk -v a="$error_rate" -v b="$ERROR_RATE_THRESHOLD" 'BEGIN{print (a > b)}')

  if [ "$latency_exceeded" -eq 1 ] || [ "$error_exceeded" -eq 1 ]; then
    log "Threshold breached (latency_exceeded=${latency_exceeded}, error_exceeded=${error_exceeded}); rolling back"
    rollback_to_stable
  else
    log "Canary metrics within thresholds; consider ramping up or marking stable."
  fi
}

rollback_to_stable() {
  # This assumes MCP has a rollback endpoint that accepts name and to_version (stable)
  # In practice, query MCP for last stable version or pass as arg.
  STABLE_VERSION="${STABLE_VERSION:-v0-stable}"
  log "Rolling back ${MODEL_NAME} to ${STABLE_VERSION}"
  payload=$(jq -n --arg name "$MODEL_NAME" --arg to_version "$STABLE_VERSION" '{name:$name,to_version:$to_version}')
  curl -s -X POST "${MCP_BASE_URL}/models/rollback" \
    -H "Content-Type: application/json" \
    -H "$(auth_header)" \
    -d "$payload" | jq .
  log "Rollback request submitted."
}

# Main flow
promote_canary
sleep 5
metrics=$(fetch_metrics)
evaluate_metrics_and_maybe_rollback "$metrics"

# End
