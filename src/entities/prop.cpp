/** @file prop.cpp
 *  @brief Prop entity class implementation.
 */

#include "prop.h"

#include "../ecs_gameplay_components.h"
#include "../ecs_world_object_components.h"
#include "../gamestate.h"
#include "../world_object_definitions.h"
#include "../libdraw_create_spritegroup.h"
#include "../mprint.h"
#include "../massert.h"

namespace rpg::entities {

entityid Prop::create(gamestate& g, proptype_t type) {
    entityid id = g.add_entity();
    const StaticWorldDefinition& definition = get_prop_definition(type);
    g.sync_entt_entity_type_tags(id, entitytype_t::PROP);
    g.registry.emplace_or_replace<NeedsUpdate>(g.ensure_registry_entity(id), NeedsUpdate{true});
    g.attach_static_world_definition(id, definition);
    return id;
}

entityid Prop::create_at(gamestate& g, proptype_t type, vec3 loc) {
    shared_ptr<dungeon_floor> df = g.d.get_floor(loc.z);
    tile_t& tile = df->tile_at(loc);
    if (!tile_is_walkable(tile.get_type())) {
        return ENTITYID_INVALID;
    }
    if (tile.entity_count() > 0) {
        return ENTITYID_INVALID;
    }
    entityid id = create(g, type);
    if (id == ENTITYID_INVALID) {
        return ENTITYID_INVALID;
    }
    entityid result = df->df_add_at(id, entitytype_t::PROP, loc);
    if (result == ENTITYID_INVALID) {
        return ENTITYID_INVALID;
    }
    g.sync_registry_grid_position(id, loc);
    return id;
}

void Prop::create_sprite(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    const entt::entity registry_entity = g.lookup_registry_entity(id);
    if (registry_entity != entt::null && g.registry.all_of<StaticVisual>(registry_entity)) {
        const StaticVisual& visual = g.registry.get<StaticVisual>(registry_entity);
        if (visual.sprites != nullptr && visual.sprite_count > 0) {
            create_sg(g, renderer, id, visual.sprites, visual.sprite_count);
            return;
        }
    }
    const proptype_t pt = g.get_component_or<PropTypeComponent>(id, proptype_t::NONE);
    const StaticWorldDefinition& def = get_prop_definition(pt);
    if (def.sprites && def.sprite_count > 0) {
        create_sg(g, renderer, id, def.sprites, def.sprite_count);
    }
}

void Prop::update_sprite(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) {
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
