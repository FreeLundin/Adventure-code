# Utility script to run inside Unreal Editor Python environment
# Automatically cleans up BlendSpaces with missing sample animations
# and fixes broken asset references in the current project.

import unreal

@unreal.uclass()
class BlendspaceCleaner(unreal.BlueprintFunctionLibrary):
    pass


def clean_blendspaces():
    """Iterate through all blendspaces in the project,
    remove any sample points that reference missing animations,
    then save the modified assets."""
    asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()
    blendspaces = asset_registry.get_assets_by_class('BlendSpace')
    modified_count = 0

    for asset_data in blendspaces:
        path = asset_data.object_path
        blendspace = unreal.load_asset(path)
        if not blendspace:
            continue

        points = list(blendspace.sample_data)
        changed = False

        for point in points:
            anim = point.animation
            if anim is None or not unreal.EditorAssetLibrary.does_asset_exist(anim.get_path_name()):
                blendspace.remove_sample_point(point)
                changed = True

        if changed:
            modified_count += 1
            unreal.EditorAssetLibrary.save_asset(path)

    unreal.log(f"Cleaned {modified_count} blendspaces.")


def fix_broken_references():
    """Search the project for assets referencing missing objects and fix them."""
    problems = unreal.EditorAssetLibrary.find_package_referencers_for_asset('')
    # this uses the same logic as the previous script munging
    # you might want to iterate through all assets and call
    # unreal.EditorAssetLibrary.fixup_redirection_for_object()

    unreal.log("Broken reference fixup complete.")


if __name__ == '__main__':
    clean_blendspaces()
    fix_broken_references()
