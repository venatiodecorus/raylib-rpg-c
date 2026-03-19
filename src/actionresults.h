/** @file actionresults.h
 *  @brief Detailed action outcome codes used by gameplay commands.
 */

#pragma once

/// @brief Detailed result codes for movement, combat, pickup, and wait actions.
enum class actionresults_t {
    MOVE_SUCCESS = 1,
    MOVE_FAIL_BLOCKED_BY_ENTITY = 2,
    MOVE_FAIL_BLOCKED_BY_WALL = 3,
    MOVE_FAIL_OUT_OF_BOUNDS = 4,
    MOVE_FAIL_NO_TILE = 5,
    MOVE_FAIL_UNKNOWN_ENTITY = 6,
    ATTACK_SUCCESS = 7,
    ATTACK_FAIL_OUT_OF_BOUNDS = 8,
    ATTACK_FAIL_NO_TILE = 9,
    PICKUP_SUCCESS = 10,
    PICKUP_FAIL_NO_TILE = 11,
    PICKUP_FAIL_NO_ENTITIES = 12,
    BLOCK_SUCCESS = 13,
    BLOCK_FAIL_UNKNOWN_ENTITY = 14,
    NONE_SUCCESS = 15,
    NONE_FAIL = 16,
    WAIT_SUCCESS = 17,
    WAIT_FAIL = 18,
    WAS_DAMAGED = 19,
};
