# This Python script can be run inside Unreal Editor's Python environment.
# It automates the wiring of the traversal ability input event for character blueprints.
# Usage: open the Output Log in the editor, paste or run this script via Script Editor or
# use "File > Execute Python Script".

import unreal

# configuration
CHAR_BP_PATHS = [
    '/Game/Blueprints/CBP_AdventureCharacter',
    '/Game/Blueprints/CBP_AdventureCharacter_Mover',
]
INPUT_ACTION = '/Game/Input/IA_HeavyAttack'  # action that should trigger traversal ability
FUNCTION_NAME = 'TryActivateAbilityByTag'
ABILITY_TAG = 'Ability.Traversal'


def bind_traversal(bp_path):
    bp = unreal.load_asset(bp_path)
    if not bp:
        unreal.log_warning(f'Blueprint not found: {bp_path}')
        return

    graph = unreal.K2EditorUtilities.get_event_graph(bp)
    if not graph:
        unreal.log_warning(f'No event graph: {bp_path}')
        return

    schema = graph.get_schema()

    # create InputAction event node
    input_node = unreal.K2Node_AddInputActionEvent()
    input_node.set_editor_property('input_action', unreal.load_asset(INPUT_ACTION))
    unreal.K2EditorUtilities.add_node(graph, input_node)

    # create call function node (TryActivateAbilityByTag)
    call_node = unreal.K2Node_CallFunction()
    call_node.set_editor_property('function_reference',
        unreal.EdGraphSchema_K2.make_function_reference(bp.generated_class, FUNCTION_NAME))
    unreal.K2EditorUtilities.add_node(graph, call_node)

    # set up tag literal
    tag_pin = call_node.find_pin('Tag')
    if tag_pin:
        tag_pin.default_value = ABILITY_TAG

    # wire the pressed execution pin to the call node exec pin
    schema.try_create_connection(input_node.find_pin('Pressed'), call_node.find_pin('execute'))

    unreal.log(f'Wired traversal input for {bp_path}')


for path in CHAR_BP_PATHS:
    bind_traversal(path)

unreal.log('Done binding traversal input')
