/** @file entity_actions.h
 *  @brief Default NPC behavior action identifiers.
 */

#pragma once

/// @brief High-level default actions that can drive NPC behavior each turn.
enum class entity_default_action_t {
    NONE = 1,
    RANDOM_MOVE,
    MOVE_TO_TARGET,
    ATTACK_TARGET_IF_ADJACENT,
    RANDOM_MOVE_AND_ATTACK_TARGET_IF_ADJACENT,
    MOVE_TO_TARGET_AND_ATTACK_TARGET_IF_ADJACENT,

    //ATTACK,
    //PICKUP,
    //BLOCK,
    //WAIT,
    COUNT
};
