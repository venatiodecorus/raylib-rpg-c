/** @file debug_entity.h
 *  @brief Small debug logging helper for printing one entity summary.
 */

#pragma once

#include "ecs_core_components.h"
#include "ecs_gameplay_components.h"
#include "entityid.h"
#include "gamestate.h"
#include "massert.h"
#include "mprint.h"

/** @brief Print a minimal debug summary for an entity id. */
static inline void debug_entity(gamestate& g, entityid id) {
    minfo("id: %d", id);
    minfo("name: %s", g.get_component_or<EntityName>(id, std::string{"no-name"}).c_str());

    entitytype_t t = (g.get_component<EntityTypeTag>(id) ? g.get_component<EntityTypeTag>(id)->type : entitytype_t::NONE);
    std::string t_s = entitytype_to_str(t);

    minfo("type: %d", t);
    minfo("type: %s", t_s.c_str());
}
