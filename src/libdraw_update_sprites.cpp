#include "libdraw_update_sprites.h"

#include "create_sg_byid.h"
#include "entitytype.h"
#include "libdraw_context.h"
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
    auto maybe_sprite_move = g.ct.get<spritemove>(id);
    if (!maybe_sprite_move.has_value()) {
        return;
    }
    Rectangle sprite_move = g.ct.get<spritemove>(id).value();
    if (sprite_move.x != 0 || sprite_move.y != 0) {
        sg->move.x = sprite_move.x;
        sg->move.y = sprite_move.y;
        entitytype_t type = g.ct.get<entitytype>(id).value_or(entitytype_t::NONE);
        massert(type != entitytype_t::NONE, "entity type is none");
        if (type == entitytype_t::PLAYER || type == entitytype_t::NPC) {
            race_t r = g.ct.get<race>(id).value_or(race_t::NONE);
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
    massert(g.ct.has<update>(id), "id %d has no update component, name %s", id, g.ct.get<name>(id).value().c_str());

    const bool do_update = g.ct.get<update>(id).value();

    if (do_update) {
        if (g.ct.has<direction>(id)) {
            const direction_t d = g.ct.get<direction>(id).value();
            libdraw_update_sprite_context_ptr(g, sg, d);
        }
    }

    libdraw_update_sprite_position(g, id, sg);

    if (g.ct.get<block_success>(id).value_or(false)) {
        libdraw_set_sg_block_success(g, renderer, id, sg);
    }

    if (g.ct.get<attacking>(id).value_or(false)) {
        libdraw_set_sg_is_attacking(g, renderer, id, sg);
    }

    const entitytype_t type = g.ct.get<entitytype>(id).value_or(entitytype_t::NONE);

    if (g.ct.get<dead>(id).value_or(false)) {
        libdraw_set_sg_is_dead(g, renderer, id, sg);
    }
    else if (g.ct.get<damaged>(id).value_or(false)) {
        libdraw_set_sg_is_damaged(g, renderer, id, sg);
    }

    if (type == entitytype_t::DOOR || type == entitytype_t::CHEST) {
        auto maybe_door_open = g.ct.get<door_open>(id);
        if (maybe_door_open.has_value()) {
            sg->set_current(maybe_door_open.value() ? 1 : 0);
        }
    }

    if (sg->update_dest()) {
        g.frame_dirty = true;
    }

    auto maybe_loc = g.ct.get<location>(id);
    if (!maybe_loc.has_value()) {
        return;
    }
    const vec3 loc = maybe_loc.value();
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
        create_sg_byid(g, renderer, i);
        libdraw_update_sprite_pre(g, renderer, i);
    }
    g.frame_dirty = true;
}

void libdraw_update_sprites_pre(gamestate& g, rpg::Renderer& renderer) {
    minfo2("BEGIN update sprites pre");
    if (g.current_scene == scene_t::GAMEPLAY) {
        libdraw_handle_dirty_entities(g, renderer);
        for (entityid id = 0; id < g.next_entityid; id++) {
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

    if (g.framecount % renderer.anim_speed != 0) {
        return;
    }

    g.frame_dirty = true;

    for (entityid id = 0; id < g.next_entityid; id++) {
        const entitytype_t type = g.ct.get<entitytype>(id).value_or(entitytype_t::NONE);
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
            const itemtype_t itype = g.ct.get<itemtype>(id).value_or(itemtype_t::NONE);
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
