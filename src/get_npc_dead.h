/** @file get_npc_dead.h
 *  @brief Convenience accessor for an NPC's dead/alive state.
 */

#pragma once

#include "ecs_gameplay_components.h"
#include "gamestate.h"

/** @brief Return whether the addressed NPC is currently marked dead. */
static inline bool get_npc_dead(gamestate& g, entityid id) {
    return g.get_component_or<DeadFlag>(id, true);
}
