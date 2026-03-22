/** @file door.cpp
 *  @brief Door entity class implementation.
 */

#include "door.h"

#include "../ecs_gameplay_components.h"
#include "../ecs_world_object_components.h"
#include "../gamestate.h"
#include "../world_object_definitions.h"
#include "../libdraw_create_spritegroup.h"
#include "../mprint.h"
#include "../massert.h"

namespace rpg::entities {

entityid Door::create(gamestate& g) {
    entityid id = g.add_entity();
    const StaticWorldDefinition& definition = get_static_world_definition(entitytype_t::DOOR);
    g.sync_entt_entity_type_tags(id, entitytype_t::DOOR);
    g.registry.emplace_or_replace<NeedsUpdate>(g.ensure_registry_entity(id), NeedsUpdate{true});
    g.attach_static_world_definition(id, definition);
    return id;
}

entityid Door::create_at(gamestate& g, vec3 loc) {
    shared_ptr<dungeon_floor> df = g.d.get_floor(loc.z);
    tile_t& tile = df->tile_at(loc);
    if (!tile_is_walkable(tile.get_type())) {
        return INVALID;
    }
    if (tile.get_type() == tiletype_t::UPSTAIRS || tile.get_type() == tiletype_t::DOWNSTAIRS) {
        return INVALID;
    }
    if (tile.entity_count() > 0) {
        return INVALID;
    }
    entityid id = create(g);
    if (id == INVALID) {
        return INVALID;
    }
    if (!df->df_add_at(id, entitytype_t::DOOR, loc)) {
        return INVALID;
    }
    g.sync_registry_grid_position(id, loc);
    g.sync_registry_open_state(id, false);
    return id;
}

bool Door::try_open(gamestate& g, entityid actor_id, vec3 loc) {
    massert(actor_id != ENTITYID_INVALID, "actor_id is invalid");
    const entityid door_id = g.tile_has_door(loc);
    if (door_id == ENTITYID_INVALID) {
        return false;
    }
    massert(g.tile_has_door(loc) == door_id, "door cache mismatch at (%d, %d, %d)", loc.x, loc.y, loc.z);
    if (g.door_is_pressure_plate_controlled(door_id)) {
        return g.messages.add("You cannot open this door with your hands");
    }
    const DoorOpenFlag* maybe_is_open = g.get_component<DoorOpenFlag>(door_id);
    massert(maybe_is_open != nullptr, "door %d has no `is_open` component", door_id);
    g.sync_registry_open_state(door_id, !maybe_is_open->value);
    g.audio.queue("sfx/Minifantasy_Dungeon_SFX/01_chest_open_1.wav");
    return true;
}

void Door::create_sprite(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    const entt::entity registry_entity = g.lookup_registry_entity(id);
    if (registry_entity != entt::null && g.registry.all_of<StaticVisual>(registry_entity)) {
        const StaticVisual& visual = g.registry.get<StaticVisual>(registry_entity);
        if (visual.sprites != nullptr && visual.sprite_count > 0) {
            create_sg(g, renderer, id, visual.sprites, visual.sprite_count);
            return;
        }
    }
    const StaticWorldDefinition& def = get_static_world_definition(entitytype_t::DOOR);
    create_sg(g, renderer, id, def.sprites, def.sprite_count);
}

void Door::update_sprite(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) {
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

void Door::draw(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    massert(sg, "spritegroup is NULL");
    Rectangle dest = {sg->dest.x, sg->dest.y, sg->dest.width, sg->dest.height};
    massert(dest.width > 0, "dest.width is 0");
    massert(dest.height > 0, "dest.height is 0");
    shared_ptr<sprite> s = sg->get_current();
    massert(s, "sprite is NULL");
    DrawTexturePro(*s->get_texture(), s->get_src(), dest, Vector2{0, 0}, 0, WHITE);
}

} // namespace rpg::entities
