/** @file libdraw_update_shield_for_entity.h
 *  @brief Helper for syncing equipped-shield animation with an entity spritegroup.
 */

#pragma once
#include "gamestate.h"
#include "libdraw_context.h"
#include "spritegroup.h"
#include "spritegroup_anim.h"

/** @brief Update an equipped shield spritegroup to match the owner's current context. */
static inline void update_shield_for_entity(gamestate& g, entityid id, spritegroup* sg) {
    minfo("update shield for entity %d", id);
    massert(id != ENTITYID_INVALID, "entity id is -1");
    massert(sg, "spritegroup is NULL");
    spritegroup* shield_sg = nullptr;
    int ctx = -1;
    const entityid shield_id = g.ct.get<equipped_shield>(id).value_or(ENTITYID_INVALID);
    if (shield_id == ENTITYID_INVALID) {
        merror2("shield_id invalid");
        return;
    }
    auto it = libdraw_ctx.spritegroups.find(shield_id);
    if (it == libdraw_ctx.spritegroups.end()) {
        merror2("shield spritegroup missing");
        return;
    }
    shield_sg = it->second.get();
    if (!shield_sg) {
        merror2("shield_sg null");
        return;
    }
    ctx = sg->sprites.at(sg->current)->get_currentcontext();
    minfo("ctx: %d", ctx);
    shield_sg->setcontexts(ctx); // what direction is shield-bearer facing?
    shield_sg->set_current(SG_ANIM_BUCKLER_SUCCESS_FRONT); //
}
