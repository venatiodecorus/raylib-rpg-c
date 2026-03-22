/** @file item.cpp
 *  @brief Item entity class implementation.
 */

#include "item.h"

#include "../ecs_gameplay_components.h"
#include "../ecs_item_components.h"
#include "../ecs_world_object_components.h"
#include "../gamestate.h"
#include "../item_definitions.h"
#include "../libdraw_create_spritegroup.h"
#include "../mprint.h"
#include "../massert.h"

namespace rpg::entities {

entityid Item::create_weapon(gamestate& g) {
    entityid id = g.add_entity();
    g.sync_entt_entity_type_tags(id, entitytype_t::ITEM);
    auto e = g.ensure_registry_entity(id);
    g.registry.emplace_or_replace<ItemSubtype>(e, ItemSubtype{itemtype_t::WEAPON});
    g.registry.emplace_or_replace<SpriteMoveState>(e, SpriteMoveState{Rectangle{0, 0, 0, 0}});
    g.registry.emplace_or_replace<NeedsUpdate>(e, NeedsUpdate{true});
    return id;
}

entityid Item::create_shield(gamestate& g) {
    entityid id = g.add_entity();
    g.sync_entt_entity_type_tags(id, entitytype_t::ITEM);
    auto e = g.ensure_registry_entity(id);
    g.registry.emplace_or_replace<ItemSubtype>(e, ItemSubtype{itemtype_t::SHIELD});
    g.registry.emplace_or_replace<ItemDurability>(e, ItemDurability{100});
    g.registry.emplace_or_replace<ItemMaxDurability>(e, ItemMaxDurability{100});
    g.registry.emplace_or_replace<NeedsUpdate>(e, NeedsUpdate{false});
    return id;
}

entityid Item::create_potion(gamestate& g) {
    entityid id = g.add_entity();
    g.sync_entt_entity_type_tags(id, entitytype_t::ITEM);
    auto e = g.ensure_registry_entity(id);
    g.registry.emplace_or_replace<ItemSubtype>(e, ItemSubtype{itemtype_t::POTION});
    g.registry.emplace_or_replace<NeedsUpdate>(e, NeedsUpdate{true});
    return id;
}

entityid Item::create_weapon_at(gamestate& g, vec3 loc) {
    if (g.d.floors.size() == 0) {
        return INVALID;
    }
    if (!g.d.is_initialized) {
        return INVALID;
    }
    if (vec3_invalid(loc)) {
        return INVALID;
    }
    shared_ptr<dungeon_floor> df = g.d.get_floor(loc.z);
    tile_t& tile = df->tile_at(loc);
    if (!tile_is_walkable(tile.get_type())) {
        return INVALID;
    }
    entityid id = create_weapon(g);
    if (id == ENTITYID_INVALID) {
        return INVALID;
    }
    if (df->df_add_at(id, entitytype_t::ITEM, loc) == ENTITYID_INVALID) {
        return INVALID;
    }
    g.registry.emplace_or_replace<Position>(g.ensure_registry_entity(id), Position{loc});
    g.sync_registry_grid_position(id, loc);
    return id;
}

entityid Item::create_shield_at(gamestate& g, vec3 loc) {
    if (g.d.floors.size() == 0) {
        return INVALID;
    }
    entityid id = create_shield(g);
    shared_ptr<dungeon_floor> df = g.d.get_floor(loc.z);
    if (!df->df_add_at(id, entitytype_t::ITEM, loc)) {
        return INVALID;
    }
    g.registry.emplace_or_replace<Position>(g.ensure_registry_entity(id), Position{loc});
    g.sync_registry_grid_position(id, loc);
    return id;
}

entityid Item::create_potion_at(gamestate& g, vec3 loc) {
    shared_ptr<dungeon_floor> df = g.d.get_floor(loc.z);
    tile_t& tile = df->tile_at(loc);
    if (!tile_is_walkable(tile.get_type())) {
        return INVALID;
    }
    entityid id = create_potion(g);
    if (id == INVALID) {
        return INVALID;
    }
    if (!df->df_add_at(id, entitytype_t::ITEM, loc)) {
        return INVALID;
    }
    auto e = g.ensure_registry_entity(id);
    g.registry.emplace_or_replace<Position>(e, Position{loc});
    g.registry.emplace_or_replace<NeedsUpdate>(e, NeedsUpdate{true});
    g.sync_registry_grid_position(id, loc);
    return id;
}

void Item::create_sprite(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    const entt::entity registry_entity = g.lookup_registry_entity(id);
    if (registry_entity != entt::null && g.registry.all_of<ItemVisual>(registry_entity)) {
        const ItemVisual& visual = g.registry.get<ItemVisual>(registry_entity);
        if (visual.sprites != nullptr && visual.sprite_count > 0) {
            create_sg(g, renderer, id, visual.sprites, visual.sprite_count);
            return;
        }
    }
    const itemtype_t itype = g.get_component_or<ItemSubtype>(id, itemtype_t::NONE);
    switch (itype) {
    case itemtype_t::POTION: {
        const potiontype_t pt = g.get_component_or<PotionSubtype>(id, potiontype_t::NONE);
        const ItemDefinition* def = find_potion_definition(pt);
        if (def) {
            create_sg(g, renderer, id, def->sprites, def->sprite_count);
        }
    } break;
    case itemtype_t::WEAPON: {
        const weapontype_t wt = g.get_component_or<WeaponSubtype>(id, weapontype_t::NONE);
        const ItemDefinition* def = find_weapon_definition(wt);
        if (def) {
            create_sg(g, renderer, id, def->sprites, def->sprite_count);
        }
    } break;
    case itemtype_t::SHIELD: {
        const shieldtype_t st = g.get_component_or<ShieldSubtype>(id, shieldtype_t::NONE);
        const ItemDefinition* def = find_shield_definition(st);
        if (def) {
            create_sg(g, renderer, id, def->sprites, def->sprite_count);
        }
    } break;
    default: break;
    }
}

void Item::update_sprite(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    massert(sg, "spritegroup is NULL");
    if (sg->update_dest()) {
        g.frame_dirty = true;
    }
    const Position* loc_ptr = g.get_component<Position>(id);
    if (!loc_ptr) {
        return;
    }
    const vec3 loc = loc_ptr->value;
    sg->snap_dest(loc.x, loc.y);
}

} // namespace rpg::entities
