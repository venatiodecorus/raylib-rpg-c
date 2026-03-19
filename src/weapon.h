/** @file weapon.h
 *  @brief Weapon subtype identifiers used by item entities and combat logic.
 */

#pragma once

/// @brief Supported weapon subtypes for equipment and combat calculations.
enum class weapontype_t {
    NONE,
    DAGGER,
    SHORT_SWORD,
    LONG_SWORD,
    GREAT_SWORD,
    AXE,
    SPEAR,
    //BOW,
    //TWO_HANDED_SWORD,
    //WARHAMMER,
    //FLAIL,
    TYPE_COUNT
};
