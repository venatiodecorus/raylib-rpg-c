/** @file shield.h
 *  @brief Shield subtype identifiers used by equipment and defense logic.
 */

#pragma once

/// @brief Supported shield subtypes for inventory and armor calculations.
enum class shieldtype_t
{
    NONE,
    BUCKLER,
    KITE,
    TOWER,
    TYPE_COUNT
};
