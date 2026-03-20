/** @file get_entity_type.h
 *  @brief Convenience accessor for an entity's high-level type component.
 */

#pragma once

#include "gamestate.h"

/** @brief Return the entity type for `id`, or `entitytype_t::NONE` when absent. */
static inline entitytype_t get_entity_type(gamestate& g, entityid id) {
    return g.ct.get_or<entitytype>(id, entitytype_t::NONE);
}
