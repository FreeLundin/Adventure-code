#!/usr/bin/env python3
# telemetry_ingest.py
# Minimal Flask telemetry ingestion service for MCP prototype.
# Usage (dev): FLASK_APP=telemetry_ingest.py flask run --host=0.0.0.0 --port=5001
# NOTE: For production, run behind a WSGI server and secure endpoints (auth, TLS).

from flask import Flask, request, jsonify
from collections import defaultdict
import time
import threading
import json
import os

app = Flask(__name__)
DATA_DIR = os.path.join(os.path.dirname(__file__), '..', 'logs')
os.makedirs(DATA_DIR, exist_ok=True)

# In-memory rolling aggregates (for demo only)
aggregates = defaultdict(lambda: {"count":0, "latency_sum":0.0, "errors":0})

LOCK = threading.Lock()
ALERT_LATENCY_MS = 100.0
ALERT_ERROR_RATE = 0.05

def persist_event(event):
    fname = os.path.join(DATA_DIR, f"telemetry_{int(time.time())}.ndjson")
    with open(fname, "a") as f:
        f.write(json.dumps(event) + "\n")

def check_alerts(model_name, model_version):
    key = f"{model_name}:{model_version}"
    with LOCK:
        agg = aggregates[key]
        if agg["count"] == 0:
            return None
        avg_latency = agg["latency_sum"] / agg["count"]
        error_rate = agg["errors"] / agg["count"]
    alerts = []
    if avg_latency > ALERT_LATENCY_MS:
        alerts.append(f"avg_latency_ms {avg_latency:.1f} > {ALERT_LATENCY_MS}")
    if error_rate > ALERT_ERROR_RATE:
        alerts.append(f"error_rate {error_rate:.3f} > {ALERT_ERROR_RATE}")
    return alerts if alerts else None

@app.route("/api/models/metrics", methods=["POST"])
def ingest_metrics():
    payload = request.get_json(force=True)
    # Expected payload: { model_name, model_version, metrics: { latency_ms, error } , telemetry_meta... }
    model_name = payload.get("model_name")
    model_version = payload.get("model_version")
    metrics = payload.get("metrics", {})
    latency = float(metrics.get("latency_ms", 0.0))
    error = bool(metrics.get("error", False))

    event = {
        "timestamp": time.time(),
        "model_name": model_name,
        "model_version": model_version,
        "latency_ms": latency,
        "error": int(error),
        "raw": payload
    }
    persist_event(event)

    key = f"{model_name}:{model_version}"
    with LOCK:
        aggregates[key]["count"] += 1
        aggregates[key]["latency_sum"] += latency
        aggregates[key]["errors"] += int(error)

    alerts = check_alerts(model_name, model_version)
    if alerts:
        # In production: push to alerting system (PagerDuty, Slack, etc.)
        app.logger.warning("ALERT for %s@%s: %s", model_name, model_version, alerts)
        return jsonify({"status":"accepted","alerts":alerts}), 200

    return jsonify({"status":"accepted"}), 200

@app.route("/api/models/metrics/summary", methods=["GET"])
def metrics_summary():
    # Return simple aggregates for debugging
    with LOCK:
        summary = {k:v for k,v in aggregates.items()}
    return jsonify(summary), 200

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5001, debug=True)
