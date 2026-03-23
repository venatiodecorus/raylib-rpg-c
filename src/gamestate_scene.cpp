/** @file gamestate_scene.cpp
 *  @brief Scene-specific utility helpers implemented on `gamestate`.
 */

#include "ecs_core_components.h"
#include "ecs_gameplay_components.h"
#include "gamestate.h"
#include <entt/entt.hpp>

void gamestate::make_all_npcs_target_player() {
    massert(hero_id != ENTITYID_INVALID, "hero_id is invalid");
    auto view = registry.view<LegacyEntityId, NpcTag>();
    for (auto entity : view) {
        entityid id = view.get<LegacyEntityId>(entity).id;
        if (auto* tgt = get_component<TargetEntity>(id)) {
            tgt->value = hero_id;
        }
    }
}
