/** @file potion.h
 *  @brief Potion subtype identifiers.
 */

#pragma once

/// @brief Supported potion variants used by inventory and consumption logic.
enum class potiontype_t
{
    NONE,
    HP_SMALL,
    HP_MEDIUM,
    HP_LARGE,
    MP_SMALL,
    MP_MEDIUM,
    MP_LARGE,
    TYPE_COUNT
};
