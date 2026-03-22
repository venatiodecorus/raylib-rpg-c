/** @file actor.h
 *  @brief Actor entity class — NPCs and player with combat, AI, sprite management.
 */

#pragma once

#include "entity_base.h"
#include "../race.h"
#include "../alignment.h"

namespace rpg::entities {

/** @brief Actor entity with NPC/player support, combat, and AI. */
class Actor : public EntityBase {
public:
    /** @brief Create an NPC entity of the requested race. */
    static entityid create_npc(gamestate& g, race_t rt);

    /** @brief Create and place an NPC at a dungeon location. */
    static entityid create_npc_at(gamestate& g, race_t rt, vec3 loc);

    /** @brief Create and place the player actor at a dungeon location. */
    static entityid create_player_at(gamestate& g, vec3 loc, const std::string& name);

    /** @brief Set NPC defaults for components. */
    static void set_defaults(gamestate& g, entityid id);

    /** @brief Roll and assign starting stat values for an NPC. */
    static void set_starting_stats(gamestate& g, entityid id);

    /** @brief Return the default alignment for a race. */
    static alignment_t default_alignment_for_race(race_t rt);

    /** @brief Return whether an alignment should default to hostile behavior. */
    static bool alignment_is_aggressive(alignment_t alignment);

    void create_sprite(gamestate& g, rpg::Renderer& renderer, entityid id) override;
    void update_sprite(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) override;
};

} // namespace rpg::entities
