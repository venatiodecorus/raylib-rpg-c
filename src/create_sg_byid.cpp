#include "create_sg_byid.h"

#include "actor_definitions.h"
#include "ecs_actor_components.h"
#include "ecs_item_components.h"
#include "ecs_world_object_components.h"
#include "entitytype.h"
#include "item.h"
#include "item_definitions.h"
#include "libdraw_create_spritegroup.h"
#include "world_object_definitions.h"

namespace {
bool create_actor_sg_from_registry(gamestate& g, rpg::Renderer& renderer, entityid id) {
    const entt::entity registry_entity = g.lookup_registry_entity(id);
    if (registry_entity == entt::null || !g.registry.all_of<ActorVisual>(registry_entity)) {
        return false;
    }

    const ActorVisual& visual = g.registry.get<ActorVisual>(registry_entity);
    if (visual.sprites == nullptr || visual.sprite_count <= 0) {
        return false;
    }

    create_sg(g, renderer, id, visual.sprites, visual.sprite_count);
    return true;
}

bool create_item_sg_from_registry(gamestate& g, rpg::Renderer& renderer, entityid id) {
    const entt::entity registry_entity = g.lookup_registry_entity(id);
    if (registry_entity == entt::null || !g.registry.all_of<ItemVisual>(registry_entity)) {
        return false;
    }

    const ItemVisual& visual = g.registry.get<ItemVisual>(registry_entity);
    if (visual.sprites == nullptr || visual.sprite_count <= 0) {
        return false;
    }

    create_sg(g, renderer, id, visual.sprites, visual.sprite_count);
    return true;
}

bool create_static_world_sg_from_registry(gamestate& g, rpg::Renderer& renderer, entityid id) {
    const entt::entity registry_entity = g.lookup_registry_entity(id);
    if (registry_entity == entt::null || !g.registry.all_of<StaticVisual>(registry_entity)) {
        return false;
    }

    const StaticVisual& visual = g.registry.get<StaticVisual>(registry_entity);
    if (visual.sprites == nullptr || visual.sprite_count <= 0) {
        return false;
    }

    create_sg(g, renderer, id, visual.sprites, visual.sprite_count);
    return true;
}
}

void create_npc_sg_byid(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");

    if (create_actor_sg_from_registry(g, renderer, id)) {
        return;
    }

    // Fallback: look up actor definition by race
    const race_t r = g.ct.get<race>(id).value_or(RACE_NONE);
    massert(r != RACE_NONE, "race is none for id %d", id);
    const ActorDefinition* def = get_actor_definition(r);
    massert(def != nullptr, "no actor definition for race %d", r);
    create_sg(g, renderer, id, def->sprites, def->sprite_count);
}

void create_door_sg_byid(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    minfo("create_door_sg_byid: %d", id);
    if (create_static_world_sg_from_registry(g, renderer, id)) {
        return;
    }
    const StaticWorldDefinition& def = get_static_world_definition(ENTITY_DOOR);
    create_sg(g, renderer, id, def.sprites, def.sprite_count);
}

void create_box_sg_byid(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    if (create_static_world_sg_from_registry(g, renderer, id)) {
        return;
    }
    const StaticWorldDefinition& def = get_static_world_definition(ENTITY_BOX);
    create_sg(g, renderer, id, def.sprites, def.sprite_count);
}

void create_chest_sg_byid(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    if (create_static_world_sg_from_registry(g, renderer, id)) {
        return;
    }
    const StaticWorldDefinition& def = get_static_world_definition(ENTITY_CHEST);
    create_sg(g, renderer, id, def.sprites, def.sprite_count);
}

void create_potion_sg_byid(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    if (create_item_sg_from_registry(g, renderer, id)) {
        return;
    }
    const potiontype_t pt = g.ct.get<potiontype>(id).value_or(POTION_NONE);
    const ItemDefinition* def = find_potion_definition(pt);
    if (def) {
        create_sg(g, renderer, id, def->sprites, def->sprite_count);
    }
}

void create_weapon_sg_byid(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    if (create_item_sg_from_registry(g, renderer, id)) {
        return;
    }
    const weapontype_t wt = g.ct.get<weapontype>(id).value_or(WEAPON_NONE);
    const ItemDefinition* def = find_weapon_definition(wt);
    if (def) {
        create_sg(g, renderer, id, def->sprites, def->sprite_count);
    }
}

void create_shield_sg_byid(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    if (create_item_sg_from_registry(g, renderer, id)) {
        return;
    }
    const shieldtype_t st = g.ct.get<shieldtype>(id).value_or(SHIELD_NONE);
    const ItemDefinition* def = find_shield_definition(st);
    if (def) {
        create_sg(g, renderer, id, def->sprites, def->sprite_count);
    }
}

void create_item_sg_byid(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    switch (g.ct.get<itemtype>(id).value_or(ITEM_NONE)) {
    case ITEM_POTION: create_potion_sg_byid(g, renderer, id); break;
    case ITEM_WEAPON: create_weapon_sg_byid(g, renderer, id); break;
    case ITEM_SHIELD: create_shield_sg_byid(g, renderer, id); break;
    default: break;
    }
}

void create_prop_sg_byid(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    if (create_static_world_sg_from_registry(g, renderer, id)) {
        return;
    }

    const proptype_t pt = g.ct.get<proptype>(id).value_or(PROP_NONE);
    const StaticWorldDefinition& def = get_prop_definition(pt);
    if (def.sprites && def.sprite_count > 0) {
        create_sg(g, renderer, id, def.sprites, def.sprite_count);
    }
}

void create_sg_byid(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    switch (g.ct.get<entitytype>(id).value_or(ENTITY_NONE)) {
    case ENTITY_PLAYER:
    case ENTITY_NPC: create_npc_sg_byid(g, renderer, id); break;
    case ENTITY_DOOR: create_door_sg_byid(g, renderer, id); break;
    case ENTITY_BOX: create_box_sg_byid(g, renderer, id); break;
    case ENTITY_CHEST: create_chest_sg_byid(g, renderer, id); break;
    case ENTITY_ITEM: create_item_sg_byid(g, renderer, id); break;
    case ENTITY_PROP: create_prop_sg_byid(g, renderer, id); break;
    default: break;
    }
}
