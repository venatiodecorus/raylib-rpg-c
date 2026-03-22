/** @file chest.h
 *  @brief Chest entity class — open/close state, inventory container, sprite management.
 */

#pragma once

#include "entity_base.h"

namespace rpg::entities {

/** @brief Chest entity with open/close state and inventory container. */
class Chest : public EntityBase {
public:
    /** @brief Create a chest entity with default components. */
    static entityid create(gamestate& g);

    /** @brief Create and place a chest entity at a dungeon location. */
    static entityid create_at(gamestate& g, vec3 loc);

    /** @brief Attempt to open a chest at the given location. */
    static bool try_open(gamestate& g, entityid actor_id, vec3 loc);

    void create_sprite(gamestate& g, rpg::Renderer& renderer, entityid id) override;
    void update_sprite(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) override;
    void draw(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) override;
};

} // namespace rpg::entities
