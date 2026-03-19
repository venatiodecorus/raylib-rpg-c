/** @file anim_type.h
 *  @brief High-level animation-type identifiers.
 */

#pragma once

/// @brief Broad animation categories used by sprite and action systems.
enum class anim_type_t {

    NONE = 0,
    IDLE,
    WALK,
    ATTACK,
    CHARGED_ATTACK,
    DMG,
    DIE,
    SPIN_DIE,
    SOUL_DIE,

};
