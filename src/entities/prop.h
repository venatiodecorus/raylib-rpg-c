/** @file prop.h
 *  @brief Prop entity class — decorative/pushable world objects with subtypes.
 */

#pragma once

#include "entity_base.h"
#include "../proptype.h"

namespace rpg::entities {

/** @brief Prop entity with 15 subtypes and optional push/pull. */
class Prop : public EntityBase {
public:
    /** @brief Create a prop entity with default components. */
    static entityid create(gamestate& g, proptype_t type);

    /** @brief Create and place a prop entity at a dungeon location. */
    static entityid create_at(gamestate& g, proptype_t type, vec3 loc);

    void create_sprite(gamestate& g, rpg::Renderer& renderer, entityid id) override;
    void update_sprite(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) override;
    void draw(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) override;
};

} // namespace rpg::entities
