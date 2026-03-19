/** @file item.h
 *  @brief High-level item category identifiers used across inventory and factories.
 */

#pragma once

/// @brief Broad item categories carried by entity components and inventory logic.
enum class itemtype_t { NONE, WEAPON, SHIELD, POTION, ARROW, QUIVER, WAND, RING, FOOD, TYPE_COUNT };
