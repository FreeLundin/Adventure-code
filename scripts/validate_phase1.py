"""
Phase 1 Completion Validator
Checks if all Phase 1 setup is complete
Run in Unreal Editor Python Console
"""

import unreal

def validate_phase1():
    """Validate Phase 1 setup is complete"""
    
    checks = {
        "Enhanced Input Assets": False,
        "Input Mapping Context": False,
        "Test Level Created": False,
        "HUD Widget Blueprint": False,
        "Character Spawned": False,
        "GAS Initialized": False,
        "WorldSettings Configured": False
    }
    
    print("=" * 60)
    print("PHASE 1 COMPLETION VALIDATOR")
    print("=" * 60)
    
    # Check 1: Enhanced Input Assets
    try:
        imc = unreal.load_asset("/Game/Input/IMC_Adventure")
        if imc:
            checks["Enhanced Input Assets"] = True
            print("✓ Enhanced Input asset found: IMC_Adventure")
    except:
        pass
    
    # Check 2: Input Mapping Context
    try:
        ia_move = unreal.load_asset("/Game/Input/IA_Move")
        ia_look = unreal.load_asset("/Game/Input/IA_Look")
        ia_sprint = unreal.load_asset("/Game/Input/IA_Sprint")
        if ia_move and ia_look and ia_sprint:
            checks["Input Mapping Context"] = True
            print("✓ Input Actions created: IA_Move, IA_Look, IA_Sprint")
    except:
        pass
    
    # Check 3: Test Level
    try:
        level = unreal.load_asset("/Game/Levels/VS_TestLevel")
        if level:
            checks["Test Level Created"] = True
            print("✓ Test level found: VS_TestLevel")
    except:
        pass
    
    # Check 4: HUD Widget Blueprint
    try:
        hud_bp = unreal.load_asset("/Game/UI/BP_HUD_Main")
        if hud_bp:
            checks["HUD Widget Blueprint"] = True
            print("✓ HUD Blueprint found: BP_HUD_Main")
    except:
        pass
    
    # Check 5: Character Blueprint
    try:
        char_bp = unreal.load_asset("/Game/Characters/ACBP_AdventureCharacter_Mover")
        if char_bp:
            checks["Character Spawned"] = True
            print("✓ Character Blueprint found: ACBP_AdventureCharacter_Mover")
    except:
        pass
    
    # Check 6: GAS Setup
    try:
        attr_set = unreal.load_asset("/Script/Adventure.UAdventureAttributeSet")
        if attr_set:
            checks["GAS Initialized"] = True
            print("✓ GAS AttributeSet initialized")
    except:
        pass
    
    # Print summary
    print("\n" + "=" * 60)
    print("VALIDATION SUMMARY")
    print("=" * 60)
    
    passed = sum(1 for v in checks.values() if v)
    total = len(checks)
    
    for check, status in checks.items():
        status_str = "✓" if status else "✗"
        print(f"{status_str} {check}")
    
    print(f"\nPassed: {passed}/{total}")
    
    if passed == total:
        print("\n✅ PHASE 1 COMPLETE - Ready for smoke test!")
    else:
        print(f"\n⏳ {total - passed} items still pending")
    
    print("=" * 60)

if __name__ == "__main__":
    validate_phase1()
