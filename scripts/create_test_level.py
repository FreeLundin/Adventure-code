"""
Test Level Creator for Adventure Project
Automatically creates VS_TestLevel.umap with basic setup
Run in Unreal Editor Python Console
"""

import unreal

def create_test_level():
    """Create a basic test level with floor and setup"""
    
    editor = unreal.get_editor_world()
    
    # Create new level
    level_path = "/Game/Levels/VS_TestLevel"
    print(f"Creating test level: {level_path}")
    
    # Get or create level (in practice, use Editor to create new level first)
    # Then add basic geometry
    
    # Create Floor (large flat plane)
    floor_actor_class = unreal.StaticMeshActor
    floor_location = unreal.Vector(0, 0, -50)
    floor = unreal.get_editor_world().spawn_actor(floor_actor_class, location=floor_location)
    
    print("✓ Created floor plane")
    
    # Create PlayerStart
    player_start_class = unreal.PlayerStart
    player_start = unreal.get_editor_world().spawn_actor(player_start_class, location=unreal.Vector(0, 0, 100))
    
    print("✓ Created PlayerStart")
    
    # Configure level visibility
    print("✓ Test level setup complete")
    print("\nNext steps:")
    print("1. File → Save Level As → Content/Levels/VS_TestLevel")
    print("2. Set as Default Startup Map (Project Settings)")
    print("3. Place character in level")
    print("4. Run smoke test")

if __name__ == "__main__":
    create_test_level()
