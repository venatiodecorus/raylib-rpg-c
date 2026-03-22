/** @file entity_renderer.h
 *  @brief Dispatch function to draw any entity type by looking up its spritegroup and calling the entity class draw() method.
 */

#pragma once

#include "entitytype.h"
#include "gamestate.h"
#include "libdraw_context.h"

#include "entities/actor.h"
#include "entities/box.h"
#include "entities/chest.h"
#include "entities/door.h"
#include "entities/item.h"
#include "entities/prop.h"

namespace rpg::entities
{

/** @brief Draw an entity of any type by dispatching to the correct entity class draw() method. */
static inline void draw(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    auto it = renderer.spritegroups.find(id);
    if (it == renderer.spritegroups.end()) {
        return;
    }
    spritegroup* sg = it->second.get();
    massert(sg, "spritegroup is NULL");
    const entitytype_t type = g.get_component<EntityTypeTag>(id) ? g.get_component<EntityTypeTag>(id)->type : entitytype_t::NONE;
    switch (type) {
    case entitytype_t::PLAYER:
    case entitytype_t::NPC: {
        Actor entity;
        entity.draw(g, renderer, id, sg);
    } break;
    case entitytype_t::BOX: {
        Box entity;
        entity.draw(g, renderer, id, sg);
    } break;
    case entitytype_t::DOOR: {
        Door entity;
        entity.draw(g, renderer, id, sg);
    } break;
    case entitytype_t::CHEST: {
        Chest entity;
        entity.draw(g, renderer, id, sg);
    } break;
    case entitytype_t::ITEM: {
        Item entity;
        entity.draw(g, renderer, id, sg);
    } break;
    case entitytype_t::PROP: {
        Prop entity;
        entity.draw(g, renderer, id, sg);
    } break;
    default: break;
    }
}

} // namespace rpg::entities
