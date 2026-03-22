/** @file door.h
 *  @brief Door entity class — open/close state, pressure plates, sprite management.
 */

#pragma once

#include "entity_base.h"

namespace rpg::entities {

/** @brief Door entity with open/close state and pressure plate linkage. */
class Door : public EntityBase {
public:
    /** @brief Create a door entity with default components. */
    static entityid create(gamestate& g);

    /** @brief Create and place a door entity at a dungeon location. */
    static entityid create_at(gamestate& g, vec3 loc);

    /** @brief Attempt to open a door at the given location. */
    static bool try_open(gamestate& g, entityid actor_id, vec3 loc);

    void create_sprite(gamestate& g, rpg::Renderer& renderer, entityid id) override;
    void update_sprite(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) override;
};

} // namespace rpg::entities
