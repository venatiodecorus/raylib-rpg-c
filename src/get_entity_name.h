/** @file get_entity_name.h
 *  @brief Convenience accessor for an entity's display name component.
 */

#pragma once


#include "gamestate.h"

/** @brief Return the entity name for `id`, or a fallback placeholder string. */
static inline string get_entity_name(gamestate& g, entityid id) {
    return g.ct.get_or<name>(id, "no-name-entity");
}
