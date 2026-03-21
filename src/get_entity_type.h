/** @file get_entity_type.h
 *  @brief Convenience accessor for an entity's high-level type component.
 */

#pragma once

#include "ecs_core_components.h"
#include "gamestate.h"

/** @brief Return the entity type for `id`, or `entitytype_t::NONE` when absent. */
static inline entitytype_t get_entity_type(gamestate& g, entityid id) {
    return (g.get_component<EntityTypeTag>(id) ? g.get_component<EntityTypeTag>(id)->type : entitytype_t::NONE);
}
