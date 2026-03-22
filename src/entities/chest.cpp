/** @file chest.cpp
 *  @brief Chest entity class implementation.
 */

#include "chest.h"

#include "../ecs_gameplay_components.h"
#include "../ecs_world_object_components.h"
#include "../gamestate.h"
#include "../world_object_definitions.h"
#include "../libdraw_create_spritegroup.h"
#include "../mprint.h"
#include "../massert.h"

namespace rpg::entities {

entityid Chest::create(gamestate& g) {
    entityid id = g.add_entity();
    const StaticWorldDefinition& definition = get_static_world_definition(entitytype_t::CHEST);
    g.sync_entt_entity_type_tags(id, entitytype_t::CHEST);
    g.registry.emplace_or_replace<NeedsUpdate>(g.ensure_registry_entity(id), NeedsUpdate{true});
    g.attach_static_world_definition(id, definition);
    return id;
}

entityid Chest::create_at(gamestate& g, vec3 loc) {
    shared_ptr<dungeon_floor> df = g.d.get_floor(loc.z);
    tile_t& tile = df->tile_at(loc);
    if (!tile_is_walkable(tile.get_type())) {
        return ENTITYID_INVALID;
    }
    if (tile.get_type() == tiletype_t::UPSTAIRS || tile.get_type() == tiletype_t::DOWNSTAIRS) {
        return ENTITYID_INVALID;
    }
    if (tile.entity_count() > 0) {
        return ENTITYID_INVALID;
    }
    entityid id = create(g);
    if (id == ENTITYID_INVALID) {
        return ENTITYID_INVALID;
    }
    if (df->df_add_at(id, entitytype_t::CHEST, loc) == ENTITYID_INVALID) {
        return ENTITYID_INVALID;
    }
    g.sync_registry_grid_position(id, loc);
    g.sync_registry_open_state(id, g.get_component_or<DoorOpenFlag>(id, false));
    return id;
}

bool Chest::try_open(gamestate& g, entityid actor_id, vec3 loc) {
    massert(actor_id != ENTITYID_INVALID, "actor_id is invalid");
    const entityid chest_id = g.tile_has_chest(loc.x, loc.y, loc.z);
    if (chest_id == ENTITYID_INVALID) {
        return false;
    }
    const DoorOpenFlag* maybe_is_open = g.get_component<DoorOpenFlag>(chest_id);
    massert(maybe_is_open != nullptr, "chest %d has no open state component", chest_id);
    if (maybe_is_open->value) {
        g.close_chest_menu();
        return true;
    }
    return g.open_chest_menu(chest_id);
}

void Chest::create_sprite(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    const entt::entity registry_entity = g.lookup_registry_entity(id);
    if (registry_entity != entt::null && g.registry.all_of<StaticVisual>(registry_entity)) {
        const StaticVisual& visual = g.registry.get<StaticVisual>(registry_entity);
        if (visual.sprites != nullptr && visual.sprite_count > 0) {
            create_sg(g, renderer, id, visual.sprites, visual.sprite_count);
            return;
        }
    }
    const StaticWorldDefinition& def = get_static_world_definition(entitytype_t::CHEST);
    create_sg(g, renderer, id, def.sprites, def.sprite_count);
}

void Chest::update_sprite(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    massert(sg, "spritegroup is NULL");
    const DoorOpenFlag* door_open_ptr = g.get_component<DoorOpenFlag>(id);
    if (door_open_ptr) {
        sg->set_current(door_open_ptr->value ? 1 : 0);
    }
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
