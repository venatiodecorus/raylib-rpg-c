/** @file entitytype.h
 *  @brief High-level entity category identifiers and label helpers.
 */

#pragma once

#include <string>

/// @brief Broad categories used to describe runtime entities.
enum class entitytype_t { NONE, PLAYER, NPC, ITEM, BOX, CHEST, DOOR, PROP, TYPE_COUNT };

/** @brief Convert an entity category into a lowercase display/debug string. */
static inline std::string entitytype_to_str(entitytype_t t) {
    switch (t) {
    case entitytype_t::NONE: return "none";
    case entitytype_t::PLAYER: return "player";
    case entitytype_t::NPC: return "npc";
    case entitytype_t::ITEM: return "item";
    case entitytype_t::BOX: return "box";
    case entitytype_t::CHEST: return "chest";
    case entitytype_t::DOOR: return "door";
    case entitytype_t::PROP: return "prop";
    case entitytype_t::TYPE_COUNT: return "entity type count";
    default: break;
    }
    return "errortype";
}
