#include "create_sg_byid.h"

#include "actor_definitions.h"
#include "ecs_actor_components.h"
#include "ecs_item_components.h"
#include "ecs_world_object_components.h"
#include "entities/actor.h"
#include "entities/box.h"
#include "entities/chest.h"
#include "entities/door.h"
#include "entities/item.h"
#include "entities/prop.h"
#include "entitytype.h"
#include "item.h"
#include "item_definitions.h"
#include "libdraw_create_spritegroup.h"
#include "world_object_definitions.h"

void create_npc_sg_byid(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    rpg::entities::Actor actor;
    actor.create_sprite(g, renderer, id);
}

void create_door_sg_byid(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    rpg::entities::Door door;
    door.create_sprite(g, renderer, id);
}

void create_box_sg_byid(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    rpg::entities::Box box;
    box.create_sprite(g, renderer, id);
}

void create_chest_sg_byid(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    rpg::entities::Chest chest;
    chest.create_sprite(g, renderer, id);
}

void create_potion_sg_byid(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    rpg::entities::Item item;
    item.create_sprite(g, renderer, id);
}

void create_weapon_sg_byid(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    rpg::entities::Item item;
    item.create_sprite(g, renderer, id);
}

void create_shield_sg_byid(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    rpg::entities::Item item;
    item.create_sprite(g, renderer, id);
}

void create_item_sg_byid(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    rpg::entities::Item item;
    item.create_sprite(g, renderer, id);
}

void create_prop_sg_byid(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    rpg::entities::Prop prop;
    prop.create_sprite(g, renderer, id);
}

void create_sg_byid(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    switch (g.get_component<EntityTypeTag>(id) ? g.get_component<EntityTypeTag>(id)->type : entitytype_t::NONE) {
    case entitytype_t::PLAYER:
    case entitytype_t::NPC: create_npc_sg_byid(g, renderer, id); break;
    case entitytype_t::DOOR: create_door_sg_byid(g, renderer, id); break;
    case entitytype_t::BOX: create_box_sg_byid(g, renderer, id); break;
    case entitytype_t::CHEST: create_chest_sg_byid(g, renderer, id); break;
    case entitytype_t::ITEM: create_item_sg_byid(g, renderer, id); break;
    case entitytype_t::PROP: create_prop_sg_byid(g, renderer, id); break;
    default: break;
    }
}
