/** @file libdraw_update_weapon_for_entity.h
 *  @brief Helper for syncing equipped-weapon animation with an entity spritegroup.
 */

#pragma once

#include "ecs_gameplay_components.h"
#include "gamestate.h"
#include "libdraw_context.h"
#include "spritegroup.h"
#include "spritegroup_anim.h"

/** @brief Update an equipped weapon spritegroup to match the owner's current context. */
static inline void update_weapon_for_entity(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) {
    massert(id != ENTITYID_INVALID, "entity id is -1");
    massert(sg, "spritegroup is NULL");
    spritegroup* w_sg = nullptr;
    entityid weaponid = ENTITYID_INVALID;
    int ctx = -1;
    weaponid = g.get_component_or<EquippedWeapon>(id, ENTITYID_INVALID);
    if (weaponid == ENTITYID_INVALID) return;
    auto it = renderer.spritegroups.find(weaponid);
    if (it == renderer.spritegroups.end()) return;
    w_sg = it->second.get();
    if (!w_sg) return;
    ctx = sg->sprites.at(sg->current)->get_currentcontext();
    w_sg->setcontexts(ctx);
    // this really should be either SLASH_F or SLASH_B
    // eventually we will select this based on other factors as well
    // or rather we'll have a better way to select which animation joins the entity's attack
    // lets test this theory by changing it to the B anim
    //spritegroup_set_current(w_sg, SG_ANIM_LONGSWORD_SLASH_B);
    w_sg->set_current(SG_ANIM_LONGSWORD_SLASH_F);
}
