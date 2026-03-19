/** @file gamestate_flag.h
 *  @brief High-level gameplay update phases used by the main loop.
 */

#pragma once

/// @brief Coarse state-machine phases for player input, animation, and NPC turns.
enum class gamestate_flag_t
{
    NONE = 0,
    PLAYER_INPUT = 1,
    PLAYER_ANIM = 2,
    NPC_TURN = 3,
    NPC_ANIM = 4,
    NPC_BATCH = 5,
    COUNT = 6
};
