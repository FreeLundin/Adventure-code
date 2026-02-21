"""
motion_matching_pipeline.py

Unreal Editor Python template for building a motion-matching manifest and
per-clip feature exports. Intended to run inside the Unreal Editor Python
environment (Editor Scripting Utilities / asset registry available).

Usage (inside Unreal Editor):
  - Open the Python console or run as an Editor utility script.
  - Adapt `collect_clips()` to locate your animation assets and export features.

This file is a lightweight template — implement project-specific retargeting
and feature extraction (root velocity, joint positions, velocities, etc.)
using the Unreal Python API (`unreal` module).
"""

def build_manifest(output_path="MotionMatchingManifest.csv"):
    """Write a minimal CSV manifest for motion-matching importers.

    Fields: asset_path, clip_name, duration, frame_count
    """
    header = "asset_path,clip_name,duration,frame_count\n"
    with open(output_path, "w", encoding="utf-8") as f:
        f.write(header)
    print(f"Wrote manifest to: {output_path}")


def collect_clips():
    """Placeholder: return list of animation assets to process.

    Replace with `unreal.AssetRegistryHelpers.get_asset_registry()` queries
    to find `/Game/Animations` clips, then export features per-clip.
    Example placeholder below — adapt to your project's paths.
    """
    return [
        '/Game/Characters/Hero/Animations/Run_01',
        '/Game/Characters/Hero/Animations/Idle_01',
    ]


if __name__ == "__main__":
    build_manifest()
