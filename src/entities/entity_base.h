/** @file entity_base.h
 *  @brief Common interface for entity class implementations.
 */

#pragma once

#include "../entityid.h"
#include "../libdraw_context.h"
#include "../spritegroup.h"

class gamestate;

namespace rpg::entities {

/** @brief Context passed to entity creation routines. */
struct CreateContext {
    gamestate& g;
    entityid id;
};

/** @brief Base interface for entity class implementations.
 *
 *  Each entity type (Box, Door, Chest, Prop, Item, Actor) implements this
 *  interface to unify entity concerns (creation, sprite, interaction, movement).
 */
class EntityBase {
public:
    virtual ~EntityBase() = default;

    /** @brief Create this entity's sprite group for rendering. */
    virtual void create_sprite(gamestate& g, rpg::Renderer& renderer, entityid id) = 0;

    /** @brief Per-frame update of sprite state (animation, direction, open/close swap). */
    virtual void update_sprite(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) = 0;

    /** @brief Draw this entity's sprite to the current render target. */
    virtual void draw(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) = 0;
};

} // namespace rpg::entities
