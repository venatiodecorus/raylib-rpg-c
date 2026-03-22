/** @file box.cpp
 *  @brief Box entity class implementation.
 */

#include "box.h"

#include "../ecs_gameplay_components.h"
#include "../ecs_world_object_components.h"
#include "../gamestate.h"
#include "../world_object_definitions.h"
#include "../libdraw_create_spritegroup.h"
#include "../mprint.h"
#include "../massert.h"

namespace rpg::entities {

entityid Box::create(gamestate& g) {
    entityid id = g.add_entity();
    const StaticWorldDefinition& definition = get_static_world_definition(entitytype_t::BOX);
    g.sync_entt_entity_type_tags(id, entitytype_t::BOX);
    auto e = g.ensure_registry_entity(id);
    g.registry.emplace_or_replace<SpriteMoveState>(e, SpriteMoveState{Rectangle{0, 0, 0, 0}});
    g.registry.emplace_or_replace<NeedsUpdate>(e, NeedsUpdate{true});
    g.registry.emplace_or_replace<PushableTag>(e, PushableTag{definition.pushable});
    g.registry.emplace_or_replace<PullableTag>(e, PullableTag{definition.pullable});
    g.registry.emplace_or_replace<SolidTag>(e, SolidTag{definition.solid});
    g.registry.emplace_or_replace<EntityName>(e, EntityName{definition.name});
    g.registry.emplace_or_replace<EntityDescription>(e, EntityDescription{definition.description});
    g.attach_static_world_definition(id, definition);
    return id;
}

entityid Box::create_at(gamestate& g, vec3 loc) {
    shared_ptr<dungeon_floor> df = g.d.get_floor(loc.z);
    tile_t& tile = df->tile_at(loc);
    if (!tile_is_walkable(tile.get_type())) {
        merror("cannot create entity on non-walkable tile");
        return ENTITYID_INVALID;
    }
    if (g.tile_has_box(loc.x, loc.y, loc.z) != ENTITYID_INVALID) {
        merror("cannot create entity on tile with box");
        return ENTITYID_INVALID;
    }
    entityid id = create(g);
    if (df->df_add_at(id, entitytype_t::BOX, loc) == ENTITYID_INVALID) {
        merror("failed df_add_at: %d, %d, %d", id, loc.x, loc.y);
        return ENTITYID_INVALID;
    }
    g.registry.emplace_or_replace<Position>(g.ensure_registry_entity(id), Position{loc});
    g.sync_registry_grid_position(id, loc);
    return id;
}

bool Box::handle_push(gamestate& g, entityid id, vec3 v) {
    bool can_push = g.get_component_or<PushableTag>(id, false);
    if (!can_push) {
        return false;
    }
    if (g.queue_state.processing_actions) {
        return g.queue_push_event(id, v);
    }
    return try_push(g, id, v);
}

bool Box::try_push(gamestate& g, entityid id, vec3 v) {
    return g.try_entity_move(id, v);
}

void Box::create_sprite(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    const entt::entity registry_entity = g.lookup_registry_entity(id);
    if (registry_entity != entt::null && g.registry.all_of<StaticVisual>(registry_entity)) {
        const StaticVisual& visual = g.registry.get<StaticVisual>(registry_entity);
        if (visual.sprites != nullptr && visual.sprite_count > 0) {
            create_sg(g, renderer, id, visual.sprites, visual.sprite_count);
            return;
        }
    }
    const StaticWorldDefinition& def = get_static_world_definition(entitytype_t::BOX);
    create_sg(g, renderer, id, def.sprites, def.sprite_count);
}

void Box::update_sprite(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) {
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
