# motion_matching_pipeline.py
#
# This script is intended to be executed within the Unreal Editor's Python
# environment (Editor Scripting).  It collects a list of animation assets and
# writes a CSV manifest that can later be consumed by an external motion-
# matching tool or pipeline.
#
# Usage (editor):
#    1. Open the project in the Editor.
#    2. Window > Python Console, then:
#          exec(open(r"<project>/Automation/motion_matching_pipeline.py").read())
#    3. Check the output file printed to the log (default: "Saved/AnimManifest.csv").
#
# A command-line version is possible using -run=pythonscript and the path.
#
# Current feature set (Phase 3 initial):
#   * Iterate all UAnimSequence assets under a configurable root folder.
#   * For each sequence: record asset path, length, sample rate, and
#     placeholder columns for root velocity, hip position, contact flags.
#   * Export a simple CSV with header and one row per clip.
#
# Expansion notes:
#   * A "real" pipeline should compute per-frame features and dump them
#     alongside the manifest (e.g. additional CSV files per clip).
#   * Retargeting notes can be read using Sequence->GetRawAnimationData(),
#     or via tag/notify parsing.
#   * This script will serve as the anchor for our motion-matching toolchain
#     and can be extended with CLI switches, filtering, and editor UI.

import unreal
import csv
import os

# configuration
SEARCH_ROOT = '/Game/Animations/Locomotion'
OUTPUT_PATH = os.path.join(unreal.SystemLibrary.get_project_directory(), 'Saved', 'AnimManifest.csv')

# find all animation sequences under the root
asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()
filter = unreal.ARFilter(
    class_names=['AnimSequence'],
    package_paths=[SEARCH_ROOT],
    recursive_paths=True
)
assets = asset_registry.get_assets(filter)

print(f"[SVGLND] Found {len(assets)} AnimSequence assets under {SEARCH_ROOT}")

# prepare CSV
with open(OUTPUT_PATH, 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    # header
    writer.writerow(['AssetPath', 'Length', 'SampleRate', 'RootMotion',
                     'HipPosition', 'ContactFlags'])
    for asset in assets:
        seq = unreal.load_asset(asset.object_path)
        if not seq:
            continue
        length = seq.get_play_length()
        rate = seq.get_frame_rate().numerator / seq.get_frame_rate().denominator
        # compute total root motion distance
        try:
            root_motion = seq.extract_root_motion(0, length).size()
        except Exception:
            root_motion = 0.0
        # compute average hip height by sampling each frame
        hip_total = 0.0
        frame_count = int(length * rate) if rate > 0 else 0
        hip_bone = 'hip'  # adjust if skeleton uses different name
        for f in range(frame_count):
            t = f / rate
            try:
                transform = seq.get_bone_transform_by_name(hip_bone, t)
                hip_total += transform.translation.Z
            except Exception:
                pass
        hip_pos = hip_total / frame_count if frame_count>0 else 0.0
        contact = 0  # bitmask or count of contact events (not implemented)
        writer.writerow([asset.object_path, length, rate, root_motion, hip_pos, contact])

        # additionally write per-frame CSV for more detailed analysis
        perframe_path = os.path.join(unreal.SystemLibrary.get_project_directory(),
                                     'Saved', f"{asset.asset_name}_frames.csv")
        with open(perframe_path, 'w', newline='') as pf:
            pfw = csv.writer(pf)
            pfw.writerow(['Frame','Time','HipZ'])
            for f in range(frame_count):
                t = f / rate
                hipz = 0.0
                try:
                    transform = seq.get_bone_transform_by_name(hip_bone, t)
                    hipz = transform.translation.Z
                except Exception:
                    pass
                pfw.writerow([f, t, hipz])

print(f"[SVGLND] Animation manifest written to {OUTPUT_PATH}")
