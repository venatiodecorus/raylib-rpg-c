/** @file event_type.h
 *  @brief Turn-scoped gameplay event categories and queue payloads.
 */

#pragma once

/// @brief Coarse result categories returned from attack resolution.
enum class attack_result_t {
    NONE,
    HIT,
    MISS,
    BLOCK,
    COUNT,
};

#include "entityid.h"
#include "vec3.h"

/**
 * @brief High-level gameplay events that may be queued and resolved in order.
 *
 * Combat follow-up ordering intentionally relies on FIFO append semantics in
 * `process_gameplay_events()`. The current contract is:
 * - attack miss: `event_type_t::ATTACK_INTENT` emits the miss message inline and does
 *   not enqueue damage/block/death/durability follow-ups
 * - attack block: `event_type_t::ATTACK_INTENT` may enqueue `event_type_t::PROVOKE_NPC`, then
 *   `event_type_t::ATTACK_BLOCK`; block resolution then appends
 *   `event_type_t::ATTACK_SHIELD_DURABILITY`
 * - attack hit: `event_type_t::ATTACK_INTENT` may enqueue `event_type_t::PROVOKE_NPC`, then
 *   `event_type_t::ATTACK_DAMAGE`; damage resolution then appends
 *   `event_type_t::ATTACK_WEAPON_DURABILITY` and, on lethal damage,
 *   `event_type_t::ATTACK_DEATH`; death resolution then appends either
 *   `event_type_t::ATTACK_AWARD_XP` plus `event_type_t::ATTACK_DROP_INVENTORY` for NPC targets
 *   or `event_type_t::ATTACK_PLAYER_DEATH` for the hero
 *
 * Future queue changes should preserve that observable order unless the tests
 * and gameplay contract are updated deliberately.
 */
enum class event_type_t {
    NONE = 0,
    MOVE_INTENT,
    PUSH_INTENT,
    ATTACK_INTENT,
    PROVOKE_NPC,
    ATTACK_BLOCK,
    ATTACK_DAMAGE,
    ATTACK_DEATH,
    ATTACK_AWARD_XP,
    ATTACK_DROP_INVENTORY,
    ATTACK_PLAYER_DEATH,
    ATTACK_WEAPON_DURABILITY,
    ATTACK_SHIELD_DURABILITY,
    PULL_INTENT,
    OPEN_DOOR_INTENT,
    OPEN_CHEST_INTENT,
    INTERACT_INTENT,
    PICKUP_INTENT,
    USE_ITEM_INTENT,
    EQUIP_ITEM_INTENT,
    DROP_ITEM_INTENT,
    CHEST_TRANSFER_INTENT,
    TRAVERSE_STAIRS_INTENT,
    REFRESH_PRESSURE_PLATES,
    PRESSURE_PLATE_SET_DOOR,
    COUNT
};

/**
 * @brief Queue payload for one gameplay event.
 *
 * The queue is intentionally narrow during the migration. Unused fields stay at
 * sentinel defaults until more event domains move over.
 */
struct gameplay_event_t {
    event_type_t type = event_type_t::NONE;
    entityid actor_id = ENTITYID_INVALID;
    entityid target_id = ENTITYID_INVALID;
    entityid item_id = ENTITYID_INVALID;
    vec3 delta = vec3{0, 0, 0};
    vec3 target_loc = vec3{-1, -1, -1};
    int floor = -1;
    int amount = 0;
    bool state = false;
    attack_result_t attack_result = attack_result_t::NONE;
};

/// @brief Resolution summary for a processed gameplay event.
struct gameplay_event_result_t {
    event_type_t type = event_type_t::NONE;
    entityid actor_id = ENTITYID_INVALID;
    bool handled = false;
    bool succeeded = false;
    attack_result_t attack_result = attack_result_t::NONE;
};
