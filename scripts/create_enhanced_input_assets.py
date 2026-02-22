#!/usr/bin/env python3
"""
Create Enhanced Input System assets for Adventure project.

This script creates:
- IMC_Adventure (Input Mapping Context)
- IA_Move, IA_Look, IA_CameraToggle, IA_Sprint, IA_Dodge, 
- IA_AttackLight, IA_AttackHeavy, IA_Interact, IA_Pause

Run in Unreal Editor Python console:
exec("C:/Unreal_Projects/Adventure/scripts/create_enhanced_input_assets.py")
"""

import unreal
import os

# Setup
CONTENT_PATH = "/Game/Input"
editor = unreal.get_editor_subsystem(unreal.UnrealEditorSubsystem)
asset_tools = unreal.AssetToolsHelpers.get_asset_tools()

def create_input_action(action_name, value_type="Axis1D"):
    """Create an Enhanced Input Action asset."""
    asset_path = f"{CONTENT_PATH}/{action_name}"
    
    # Check if already exists
    if unreal.EditorAssetLibrary.does_asset_exist(asset_path):
        unreal.log(f"Asset already exists: {action_name}")
        return unreal.load_asset(asset_path)
    
    # Create new Input Action
    action = unreal.InputAction()
    action.value_type = unreal.InputActionValueType[value_type] if value_type != "Digital" else unreal.InputActionValueType.DIGITAL
    
    # Save the asset
    unreal.EditorAssetLibrary.save_asset(asset_path)
    unreal.log(f"Created Input Action: {action_name}")
    return unreal.load_asset(asset_path)

def create_input_mapping_context(imc_name):
    """Create an Enhanced Input Mapping Context asset."""
    asset_path = f"{CONTENT_PATH}/{imc_name}"
    
    # Check if already exists
    if unreal.EditorAssetLibrary.does_asset_exist(asset_path):
        unreal.log(f"Asset already exists: {imc_name}")
        return unreal.load_asset(asset_path)
    
    # Create new IMC
    imc = unreal.InputMappingContext()
    
    # Save the asset - note: we'll bind actions after creation
    unreal.EditorAssetLibrary.save_asset(asset_path)
    unreal.log(f"Created Input Mapping Context: {imc_name}")
    return unreal.load_asset(asset_path)

# Ensure Input content directory exists
if not unreal.EditorAssetLibrary.does_directory_exist(CONTENT_PATH):
    unreal.EditorAssetLibrary.create_directory(CONTENT_PATH)
    unreal.log(f"Created directory: {CONTENT_PATH}")

# Create Input Actions with appropriate value types
input_actions_config = [
    ("IA_Move", "Axis2D"),           # 2D vector for movement
    ("IA_Look", "Axis2D"),           # 2D vector for camera look
    ("IA_CameraToggle", "Digital"),  # On/Off for mode cycling
    ("IA_Sprint", "Digital"),        # On/Off for sprint
    ("IA_Dodge", "Digital"),         # On/Off for dodge
    ("IA_AttackLight", "Digital"),   # On/Off for light attack
    ("IA_AttackHeavy", "Digital"),   # On/Off for heavy attack
    ("IA_Interact", "Digital"),      # On/Off for interact
    ("IA_Pause", "Digital"),         # On/Off for pause
]

unreal.log("=" * 60)
unreal.log("Creating Enhanced Input Assets...")
unreal.log("=" * 60)

# Create all Input Actions
for action_name, value_type in input_actions_config:
    create_input_action(action_name, value_type)

# Create Input Mapping Context
imc = create_input_mapping_context("IMC_Adventure")

unreal.log("=" * 60)
unreal.log("Enhanced Input Asset Creation Complete!")
unreal.log(f"IMC created at: {CONTENT_PATH}/IMC_Adventure")
unreal.log(f"Input Actions created at: {CONTENT_PATH}/IA_*")
unreal.log("=" * 60)
unreal.log("\nNext steps:")
unreal.log("1. Open IMC_Adventure in the editor")
unreal.log("2. Add mappings for each IA_* action:")
unreal.log("   - IA_Move: W/A/S/D keys (gamepad left stick)")
unreal.log("   - IA_Look: Mouse X/Y (gamepad right stick)")
unreal.log("   - IA_CameraToggle: Mouse Wheel Y")
unreal.log("   - IA_Sprint: Left Shift")
unreal.log("   - IA_Dodge: Space")
unreal.log("   - IA_AttackLight: Left Mouse Button")
unreal.log("   - IA_AttackHeavy: Right Mouse Button")
unreal.log("   - IA_Interact: E")
unreal.log("   - IA_Pause: P")
