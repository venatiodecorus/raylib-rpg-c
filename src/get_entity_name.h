/** @file get_entity_name.h
 *  @brief Convenience accessor for an entity's display name component.
 */

#pragma once


#include "ecs_gameplay_components.h"
#include "gamestate.h"

/** @brief Return the entity name for `id`, or a fallback placeholder string. */
static inline string get_entity_name(gamestate& g, entityid id) {
    return g.get_component_or<EntityName>(id, std::string{"no-name-entity"});
}
