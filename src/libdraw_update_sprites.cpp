#include "libdraw_update_sprites.h"

#include "ecs_actor_components.h"
#include "ecs_gameplay_components.h"
#include "ecs_item_components.h"
#include "entities/entity_base.h"
#include "set_sg.h"
#include "spritegroup_anim.h"
#include "update_sprite.h"

void libdraw_update_sprite_context_ptr(gamestate& g, spritegroup* group, direction_t dir) {
    massert(group != NULL, "group is NULL");
    int old_ctx = group->sprites.at(group->current)->get_currentcontext();
    int ctx = old_ctx;
    ctx = dir == direction_t::NONE                                      ? old_ctx
          : dir == direction_t::DOWN_RIGHT                              ? SPRITEGROUP_CONTEXT_R_D
          : dir == direction_t::DOWN_LEFT                               ? SPRITEGROUP_CONTEXT_L_D
          : dir == direction_t::UP_RIGHT                                ? SPRITEGROUP_CONTEXT_R_U
          : dir == direction_t::UP_LEFT                                 ? SPRITEGROUP_CONTEXT_L_U
          : dir == direction_t::DOWN && ctx == SPRITEGROUP_CONTEXT_R_D  ? SPRITEGROUP_CONTEXT_R_D
          : dir == direction_t::DOWN && ctx == SPRITEGROUP_CONTEXT_L_D  ? SPRITEGROUP_CONTEXT_L_D
          : dir == direction_t::DOWN && ctx == SPRITEGROUP_CONTEXT_R_U  ? SPRITEGROUP_CONTEXT_R_D
          : dir == direction_t::DOWN && ctx == SPRITEGROUP_CONTEXT_L_U  ? SPRITEGROUP_CONTEXT_L_D
          : dir == direction_t::UP && ctx == SPRITEGROUP_CONTEXT_R_D    ? SPRITEGROUP_CONTEXT_R_U
          : dir == direction_t::UP && ctx == SPRITEGROUP_CONTEXT_L_D    ? SPRITEGROUP_CONTEXT_L_U
          : dir == direction_t::UP && ctx == SPRITEGROUP_CONTEXT_R_U    ? SPRITEGROUP_CONTEXT_R_U
          : dir == direction_t::UP && ctx == SPRITEGROUP_CONTEXT_L_U    ? SPRITEGROUP_CONTEXT_L_U
          : dir == direction_t::RIGHT && ctx == SPRITEGROUP_CONTEXT_R_D ? SPRITEGROUP_CONTEXT_R_D
          : dir == direction_t::RIGHT && ctx == SPRITEGROUP_CONTEXT_L_D ? SPRITEGROUP_CONTEXT_R_D
          : dir == direction_t::RIGHT && ctx == SPRITEGROUP_CONTEXT_R_U ? SPRITEGROUP_CONTEXT_R_U
          : dir == direction_t::RIGHT && ctx == SPRITEGROUP_CONTEXT_L_U ? SPRITEGROUP_CONTEXT_R_U
          : dir == direction_t::LEFT && ctx == SPRITEGROUP_CONTEXT_R_D  ? SPRITEGROUP_CONTEXT_L_D
          : dir == direction_t::LEFT && ctx == SPRITEGROUP_CONTEXT_L_D  ? SPRITEGROUP_CONTEXT_L_D
          : dir == direction_t::LEFT && ctx == SPRITEGROUP_CONTEXT_R_U  ? SPRITEGROUP_CONTEXT_L_U
          : dir == direction_t::LEFT && ctx == SPRITEGROUP_CONTEXT_L_U  ? SPRITEGROUP_CONTEXT_L_U
                                                                        : old_ctx;
    if (ctx != old_ctx) {
        g.frame_dirty = true;
    }
    group->setcontexts(ctx);
}

void libdraw_update_sprite_position(gamestate& g, entityid id, spritegroup* sg) {
    massert(sg, "spritegroup is NULL");
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    const SpriteMoveState* sprite_move_ptr = g.get_component<SpriteMoveState>(id);
    if (!sprite_move_ptr) {
        return;
    }
    Rectangle sprite_move = sprite_move_ptr->value;
    if (sprite_move.x != 0 || sprite_move.y != 0) {
        sg->move.x = sprite_move.x;
        sg->move.y = sprite_move.y;
        entitytype_t type = (g.get_component<EntityTypeTag>(id) ? g.get_component<EntityTypeTag>(id)->type : entitytype_t::NONE);
        massert(type != entitytype_t::NONE, "entity type is none");
        if (type == entitytype_t::PLAYER || type == entitytype_t::NPC) {
            race_t r = (g.get_component<ActorKind>(id) ? g.get_component<ActorKind>(id)->race : race_t::NONE);
            if (r == race_t::BAT) {
                sg->current = SG_ANIM_BAT_IDLE;
            }
            else if (r == race_t::GREEN_SLIME) {
                sg->current = SG_ANIM_SLIME_IDLE;
            }
            else {
                sg->current = SG_ANIM_NPC_WALK;
            }
        }
        g.frame_dirty = true;
    }
}

void libdraw_update_sprite_ptr(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) {
    minfo3("Begin update sprite ptr: %d", id);
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    massert(sg, "spritegroup is NULL");
    massert(g.has_component<NeedsUpdate>(id), "id %d has no update component, name %s", id, g.get_component_or<EntityName>(id, std::string{"unknown"}).c_str());

    const NeedsUpdate* update_ptr = g.get_component<NeedsUpdate>(id);
    const bool do_update = update_ptr ? update_ptr->value : false;

    if (do_update) {
        const Facing* facing_ptr = g.get_component<Facing>(id);
        if (facing_ptr) {
            const direction_t d = facing_ptr->value;
            libdraw_update_sprite_context_ptr(g, sg, d);
        }
    }

    libdraw_update_sprite_position(g, id, sg);

    if (g.get_component_or<BlockSuccessFlag>(id, false)) {
        libdraw_set_sg_block_success(g, renderer, id, sg);
    }

    if (g.get_component_or<AttackingFlag>(id, false)) {
        libdraw_set_sg_is_attacking(g, renderer, id, sg);
    }

    const entitytype_t type = (g.get_component<EntityTypeTag>(id) ? g.get_component<EntityTypeTag>(id)->type : entitytype_t::NONE);

    if (g.get_component_or<DeadFlag>(id, false)) {
        libdraw_set_sg_is_dead(g, renderer, id, sg);
    }
    else if (g.get_component_or<DamagedFlag>(id, false)) {
        libdraw_set_sg_is_damaged(g, renderer, id, sg);
    }

    rpg::entities::EntityBase::for_type(type).update_sprite(g, renderer, id, sg);

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

void libdraw_update_sprite_pre(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    auto it = renderer.spritegroups.find(id);
    if (it != renderer.spritegroups.end()) {
        libdraw_update_sprite_ptr(g, renderer, id, it->second.get());
    }
}

void libdraw_handle_dirty_entities(gamestate& g, rpg::Renderer& renderer) {
    if (!g.dirty_entities) {
        return;
    }
    for (entityid i = g.new_entityid_begin; i < g.new_entityid_end; i++) {
        const entitytype_t itype = g.get_component<EntityTypeTag>(i) ? g.get_component<EntityTypeTag>(i)->type : entitytype_t::NONE;
        rpg::entities::EntityBase::for_type(itype).create_sprite(g, renderer, i);
        libdraw_update_sprite_pre(g, renderer, i);
    }
    g.frame_dirty = true;
}

void libdraw_update_sprites_pre(gamestate& g, rpg::Renderer& renderer) {
    minfo2("BEGIN update sprites pre");
    if (g.current_scene == scene_t::GAMEPLAY) {
        libdraw_handle_dirty_entities(g, renderer);
        auto view = g.registry.view<LegacyEntityId, EntityTypeTag>();
        for (auto entity : view) {
            entityid id = view.get<LegacyEntityId>(entity).id;
            libdraw_update_sprite_pre(g, renderer, id);
        }
    }
    msuccess2("END update sprites pre");
}

void libdraw_update_sprites_post(gamestate& g, rpg::Renderer& renderer) {
    if (g.current_scene != scene_t::GAMEPLAY) {
        g.frame_dirty = false;
        return;
    }

    float dt = g.test ? (1.0f / DEFAULT_TARGET_FPS) : GetFrameTime();
    renderer.anim_elapsed += dt;
    if (renderer.anim_elapsed < ANIM_FRAME_TIME) {
        return;
    }
    renderer.anim_elapsed -= ANIM_FRAME_TIME;

    g.frame_dirty = true;

    auto view = g.registry.view<LegacyEntityId, EntityTypeTag>();
    for (auto entity : view) {
        entityid id = view.get<LegacyEntityId>(entity).id;
        const entitytype_t type = view.get<EntityTypeTag>(entity).type;
        if (type == entitytype_t::NONE) {
            continue;
        }

        auto it = renderer.spritegroups.find(id);
        spritegroup* sg = it != renderer.spritegroups.end() ? it->second.get() : nullptr;
        if (!sg) {
            continue;
        }

        auto s = sg->sprites.at(sg->current);
        if (!s) {
            continue;
        }

        s->incr_frame();

        if (type == entitytype_t::PLAYER || type == entitytype_t::NPC) {
            if (s->get_num_loops() >= 1) {
                sg->current = sg->default_anim;
                s->set_num_loops(0);
            }
        }
        else if (type == entitytype_t::ITEM) {
            const itemtype_t itype = g.get_component_or<ItemSubtype>(id, itemtype_t::NONE);
            switch (itype) {
            case itemtype_t::WEAPON: {
                if (sg->current == 0) {
                    break;
                }
                auto s2 = sg->sprites.at(sg->current + 1);
                if (!s2) {
                    break;
                }
                s2->incr_frame();
                g.frame_dirty = true;
                if (s2->get_num_loops() >= 1) {
                    sg->current = sg->default_anim;
                    s2->set_num_loops(0);
                }
            } break;
            case itemtype_t::SHIELD: {
                if (sg->current == 0) {
                    break;
                }
                minfo("shield appears to be equipped...");
                auto s2 = sg->sprites.at(sg->current + 1);
                if (!s2) {
                    break;
                }
                s2->incr_frame();
                g.frame_dirty = true;
                if (s2->get_num_loops() >= 1) {
                    sg->current = sg->default_anim;
                    s2->set_num_loops(0);
                }
            } break;
            default: break;
            }
        }
    }
}
