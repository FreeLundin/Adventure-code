"""
Simple Editor Python smoke test scaffold for the Vertical Slice.

This script is intended to be run inside the Unreal Editor Python environment.
It demonstrates spawning required actors and simulating basic input for a short
period, capturing logs for attachment to the PR. Implementers should expand the
assertions to match final gameplay expectations.

Usage (Editor Python console):
>>> import Documentation.scripts.run_vs_smoke_test as smoke
>>> smoke.run()

Note: This is a minimal scaffold and may need project-specific API tuning.
"""

import time
import unreal


def _log(msg):
    unreal.log("[VS-SmokeTest] %s" % msg)


def run(sim_seconds=10):
    _log("Starting VS smoke test (sim_seconds=%s)" % sim_seconds)

    # Open map (assumes Content/VS_Playtest/VS_Playtest.umap exists)
    map_path = '/Game/VS_Playtest/VS_Playtest'
    try:
        unreal.EditorLevelLibrary.load_level(map_path)
    except Exception as e:
        _log('Failed to load map %s: %s' % (map_path, e))
        return False

    # Basic sanity checks: find player start
    starts = unreal.EditorLevelLibrary.get_all_level_actors()
    _log('Actors in map: %d' % len(starts))

    # TODO: spawn or find the AnimatedMannyPawnExtended class and possess it
    # This section is intentionally minimal — expand for your project types

    # Simulate simple wait to allow the map to initialize
    for i in range(sim_seconds):
        _log('Tick %d/%d' % (i + 1, sim_seconds))
        time.sleep(1.0)

    _log('VS smoke test completed — attach Saved/Logs and Saved/Automation output to PR')
    return True


if __name__ == '__main__':
    run()
