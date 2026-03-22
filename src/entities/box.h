/** @file box.h
 *  @brief Box entity class — push/pull physics and sprite management.
 */

#pragma once

#include "entity_base.h"

namespace rpg::entities {

/** @brief Box entity with push/pull physics. */
class Box : public EntityBase {
public:
    /** @brief Create a box entity with default components. */
    static entityid create(gamestate& g);

    /** @brief Create and place a box entity at a dungeon location. */
    static entityid create_at(gamestate& g, vec3 loc);

    /** @brief Handle pushing a box in a direction. */
    static bool handle_push(gamestate& g, entityid id, vec3 v);

    /** @brief Try to push an entity in a direction. */
    static bool try_push(gamestate& g, entityid id, vec3 v);

    void create_sprite(gamestate& g, rpg::Renderer& renderer, entityid id) override;
    void update_sprite(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) override;
    void draw(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) override;
};

} // namespace rpg::entities
