#include "set_sg.h"

#include "libdraw_update_shield_for_entity.h"
#include "libdraw_update_weapon_for_entity.h"
#include "spritegroup_anim.h"

void libdraw_set_sg_block_success(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* const sg) {
    minfo("set sg block success");
    massert(id != ENTITYID_INVALID, "entity id is -1");
    massert(sg, "spritegroup is NULL");
    const race_t r = g.ct.get<race>(id).value_or(race_t::NONE);
    if (r == race_t::GREEN_SLIME) {
        minfo("setting SG_ANIM_SLIME_IDLE");
        sg->set_current(SG_ANIM_SLIME_IDLE);
    }
    else if (r == race_t::WOLF) {
        minfo("setting SG_ANIM_WOLF_IDLE");
        sg->set_current(SG_ANIM_WOLF_IDLE);
    }
    else {
        minfo("setting SG_ANIM_NPC_GUARD_SUCCESS");
        sg->set_current(SG_ANIM_NPC_GUARD_SUCCESS);
    }
    update_shield_for_entity(g, renderer, id, sg);
}

void libdraw_set_sg_is_damaged(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* const sg) {
    massert(id != ENTITYID_INVALID, "entity id is -1");
    massert(sg, "spritegroup is NULL");
    const race_t r = g.ct.get<race>(id).value_or(race_t::NONE);
    const int anim_index = r == race_t::GREEN_SLIME ? SG_ANIM_SLIME_DMG : SG_ANIM_NPC_DMG;
    sg->set_current(anim_index);
}

void libdraw_set_sg_is_dead(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* const sg) {
    massert(id != ENTITYID_INVALID, "entity id is -1");
    massert(sg, "spritegroup is NULL");

    if (!g.ct.has<dead>(id)) {
        return;
    }

    if (!g.ct.get<dead>(id).value()) {
        return;
    }

    const race_t r = g.ct.get<race>(id).value();
    if (r == race_t::NONE) {
        return;
    }

    const int anim_index = r == race_t::BAT           ? SG_ANIM_BAT_DIE
                           : r == race_t::GREEN_SLIME ? SG_ANIM_SLIME_DIE
                           : r == race_t::SKELETON    ? SG_ANIM_SKELETON_DIE
                           : r == race_t::RAT         ? SG_ANIM_RAT_DIE
                           : r == race_t::ZOMBIE      ? SG_ANIM_ZOMBIE_DIE
                                                   : SG_ANIM_NPC_SPINDIE;
    if (sg->current == anim_index) {
        return;
    }

    sg->set_default_anim(anim_index);
    sg->set_current(sg->default_anim);
    sg->set_stop_on_last_frame(true);
}

void libdraw_set_sg_is_attacking(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* const sg) {
    massert(id != ENTITYID_INVALID, "entityid is invalid");
    massert(sg, "spritegroup is NULL");
    const race_t r = g.ct.get<race>(id).value_or(race_t::NONE);
    massert(r != race_t::NONE, "race cant be none");
    sg->set_current(
        r == race_t::GREEN_SLIME ? SG_ANIM_SLIME_JUMP_ATTACK
        : r == race_t::SKELETON  ? SG_ANIM_SKELETON_ATTACK
        : r == race_t::RAT       ? SG_ANIM_RAT_ATTACK
        : r == race_t::ZOMBIE    ? SG_ANIM_ZOMBIE_ATTACK
        : r == race_t::BAT       ? SG_ANIM_BAT_ATTACK
                              : SG_ANIM_NPC_ATTACK);
    update_weapon_for_entity(g, renderer, id, sg);
}
