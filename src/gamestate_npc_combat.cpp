#include "ecs_actor_components.h"
#include "ecs_core_components.h"
#include "ecs_gameplay_components.h"
#include "gamestate.h"
#include <entt/entt.hpp>

/** @file gamestate_npc_combat.cpp
 *  @brief Combat, NPC behavior, and pathing helpers implemented on `gamestate`.
 */

int gamestate::compute_armor_class(entityid id) {
    massert(ENTITYID_INVALID != id, "id is invalid");
    int base_armor_class = get_component_or<BaseArmorClass>(id, 10);
    int dex_bonus = get_stat_bonus(get_component_or<DexterityAttr>(id, 10));
    int total_ac = base_armor_class + dex_bonus;
    return total_ac;
}

bool gamestate::compute_attack_roll(entityid attacker, entityid target) {
    int str = get_component_or<StrengthAttr>(attacker, 10);
    int bonus = get_stat_bonus(str);
    uniform_int_distribution<int> gen(1, 20);
    int roll = gen(mt) + bonus;
    int ac = compute_armor_class(target);
    return roll >= ac;
}

int gamestate::compute_attack_damage(entityid attacker, entityid target) {
    int str = get_component_or<StrengthAttr>(attacker, 10);
    int bonus = std::max(0, get_stat_bonus(str));
    entityid equipped_wpn = get_component_or<EquippedWeapon>(attacker, ENTITYID_INVALID);
    vec3 dmg_range = get_component_or<DamageDice>(equipped_wpn, MINIMUM_DAMAGE);
    uniform_int_distribution<int> gen(dmg_range.x, dmg_range.y);
    int dmg = std::max(1, gen(mt));
    return dmg + bonus;
}

void gamestate::add_combat_miss_message(entityid attacker_id, entityid target_id) {
    const string atk_name = get_component_or<EntityName>(attacker_id, "no-name");
    const string tgt_name = get_component_or<EntityName>(target_id, "no-name");
    messages.add_history("%s swings at %s and misses!", atk_name.c_str(), tgt_name.c_str());
}

void gamestate::add_combat_block_message(entityid attacker_id, entityid target_id) {
    const string atk_name = get_component_or<EntityName>(attacker_id, "no-name");
    const string tgt_name = get_component_or<EntityName>(target_id, "no-name");
    messages.add_history("%s blocked an attack from %s", tgt_name.c_str(), atk_name.c_str());
}

void gamestate::add_combat_damage_message(entityid attacker_id, entityid target_id, int damage) {
    const string atk_name = get_component_or<EntityName>(attacker_id, "no-name");
    const string tgt_name = get_component_or<EntityName>(target_id, "no-name");
    messages.add_history("%s deals %d damage to %s", atk_name.c_str(), damage, tgt_name.c_str());
}

void gamestate::add_combat_break_message(entityid item_id) {
    messages.add_history("%s broke!", get_component_or<EntityName>(item_id, "no-name-item").c_str());
}

void gamestate::add_combat_player_death_message() {
    messages.add("You died");
}

void gamestate::handle_weapon_durability_loss(entityid atk_id, entityid tgt_id) {
    entityid equipped_wpn = get_component_or<EquippedWeapon>(atk_id, ENTITYID_INVALID);
    const ItemDurability* dura_ptr = get_component<ItemDurability>(equipped_wpn);
    if (!dura_ptr) {
        return;
    }
    int dura = dura_ptr->value;
    if (dura > 0) {
        return;
    }
    remove_from_inventory(atk_id, equipped_wpn);
    auto wpn_entity = lookup_registry_entity(equipped_wpn);
    if (wpn_entity != entt::null) {
        registry.destroy(wpn_entity);
    }
    bool event_heard = check_hearing(hero_id, get_component_or<Position>(tgt_id, vec3{-1, -1, -1}));
    if (event_heard) {
        audio.queue("sfx/Minifantasy_Dungeon_SFX/26_sword_hit_1.wav");
    }
    add_combat_break_message(equipped_wpn);
}

void gamestate::handle_shield_durability_loss(entityid defender, entityid attacker) {
    entityid shield = get_component_or<EquippedShield>(defender, ENTITYID_INVALID);
    const ItemDurability* dura_ptr = get_component<ItemDurability>(shield);
    if (!dura_ptr) {
        return;
    }
    int dura = dura_ptr->value;
    if (dura > 0) {
        return;
    }
    remove_from_inventory(defender, shield);
    auto shield_entity = lookup_registry_entity(shield);
    if (shield_entity != entt::null) {
        registry.destroy(shield_entity);
    }
    bool event_heard = check_hearing(hero_id, get_component_or<Position>(defender, vec3{-1, -1, -1}));
    if (event_heard) {
        audio.queue("sfx/Minifantasy_Dungeon_SFX/26_sword_hit_1.wav");
    }
    add_combat_break_message(shield);
}

int gamestate::get_npc_xp(entityid id) {
    return get_component_or<Experience>(id, 0);
}

void gamestate::update_npc_xp(entityid id, [[maybe_unused]] entityid target_id) {
    int old_xp = get_npc_xp(id);
    int reward_xp = 1;
    int new_xp = old_xp + reward_xp;
    if (auto* xp_comp = get_component<Experience>(id)) {
        xp_comp->value = new_xp;
    }
    maybe_unlock_level_up(id);
}

void gamestate::provoke_npc(entityid npc_id, entityid source_id) {
    if (npc_id == ENTITYID_INVALID) {
        return;
    }
    if ((get_component<EntityTypeTag>(npc_id) ? get_component<EntityTypeTag>(npc_id)->type : entitytype_t::NONE) != entitytype_t::NPC) {
        return;
    }
    if (get_component_or<DeadFlag>(npc_id, true)) {
        return;
    }

    const entityid target = source_id != ENTITYID_INVALID ? source_id : hero_id;
    if (target != ENTITYID_INVALID) {
    }

    update_npc_behavior(npc_id);
}

void gamestate::handle_shield_block_sfx(entityid target_id) {
    bool event_heard = check_hearing(hero_id, get_component_or<Position>(target_id, vec3{-1, -1, -1}));
    if (event_heard) {
        audio.queue("sfx/Minifantasy_Dungeon_SFX/26_sword_hit_3.wav");
    }
}

attack_result_t gamestate::resolve_attack_intent(entityid attacker_id, vec3 target_loc) {
    massert(attacker_id != ENTITYID_INVALID, "attacker is NULL");
    massert(!get_component_or<DeadFlag>(attacker_id, false), "attacker entity is dead");
    massert(has_component<Position>(attacker_id), "entity %d has no location", attacker_id);

    // Ordering contract for queued combat:
    // 1. `event_type_t::ATTACK_INTENT` decides miss/block/hit and appends the first
    //    follow-up events only; it does not apply chained side effects inline.
    // 2. For NPC targets, provoke is appended before block/damage so aggro is
    //    established ahead of later combat consequences in the same turn slice.
    // 3. Block appends shield durability loss after the block message/state.
    // 4. Damage appends weapon durability loss after the damage message/state,
    //    and appends death only if HP reached zero or below.
    // 5. Death appends XP/drop or player-death consequences after the corpse
    //    has been moved to the tile's dead-NPC cache.
    //
    // Tests and plan docs assume this FIFO append order remains stable.
    vec3 attacker_loc = get_component<Position>(attacker_id)->value;
    shared_ptr<dungeon_floor> df = d.get_floor(attacker_loc.z);
    tile_t& tile = df->tile_at(vec3{target_loc.x, target_loc.y, attacker_loc.z});
    const int dx = target_loc.x - attacker_loc.x;
    const int dy = target_loc.y - attacker_loc.y;
    if (auto* facing = get_component<Facing>(attacker_id)) {
        facing->value = get_dir_from_xy(dx, dy);
    }
    if (auto* atk = get_component<AttackingFlag>(attacker_id)) {
        atk->value = true;
    }
    if (auto* upd = get_component<NeedsUpdate>(attacker_id)) {
        upd->value = true;
    }

    const entityid target_id = tile.get_cached_live_npc();
    if (target_id == INVALID) {
        return attack_result_t::MISS;
    }

    const entitytype_t type = (get_component<EntityTypeTag>(target_id) ? get_component<EntityTypeTag>(target_id)->type : entitytype_t::NONE);
    if (type != entitytype_t::PLAYER && type != entitytype_t::NPC) {
        return attack_result_t::MISS;
    }
    if (get_component_or<DeadFlag>(target_id, true)) {
        return attack_result_t::MISS;
    }
    if (type == entitytype_t::NPC) {
        queue_provoke_npc_event(target_id, attacker_id);
    }

    if (!compute_attack_roll(attacker_id, target_id)) {
        add_combat_miss_message(attacker_id, target_id);
        return attack_result_t::MISS;
    }

    const entityid shield_id = get_component_or<EquippedShield>(target_id, ENTITYID_INVALID);
    if (shield_id != ENTITYID_INVALID) {
        uniform_int_distribution<int> gen(1, MAX_BLOCK_CHANCE);
        const int roll = gen(mt);
        const int chance = get_component_or<BlockChance>(shield_id, MAX_BLOCK_CHANCE);
        const int low_roll = MAX_BLOCK_CHANCE - chance;
        if (roll > low_roll) {
            queue_attack_block_event(attacker_id, target_id);
            return attack_result_t::BLOCK;
        }
    }

    queue_attack_damage_event(attacker_id, target_id, compute_attack_damage(attacker_id, target_id));
    return attack_result_t::HIT;
}

void gamestate::resolve_attack_block_event(entityid attacker_id, entityid target_id) {
    if (attacker_id == ENTITYID_INVALID || target_id == ENTITYID_INVALID) {
        return;
    }
    queue_attack_shield_durability_event(target_id, attacker_id);
    handle_shield_block_sfx(target_id);
    add_combat_block_message(attacker_id, target_id);
}

void gamestate::resolve_attack_damage_event(entityid attacker_id, entityid target_id, int damage) {
    if (attacker_id == ENTITYID_INVALID || target_id == ENTITYID_INVALID) {
        return;
    }
    if (get_component_or<DeadFlag>(target_id, false)) {
        return;
    }

    const HitPoints* tgt_hp_ptr = get_component<HitPoints>(target_id);
    if (!tgt_hp_ptr) {
        merror("target has no HP component");
        return;
    }

    vec2 tgt_hp = tgt_hp_ptr->value;
    tgt_hp.x -= damage;
    add_combat_damage_message(attacker_id, target_id, damage);
    {
        const Position* popup_loc = get_component<Position>(target_id);
        if (popup_loc) {
            const vec3 loc = popup_loc->value;
            damage_popups_sys.add(loc.x, loc.y, loc.z, damage, false, mt);
            frame_dirty = true;
        }
    }
    queue_attack_weapon_durability_event(attacker_id, target_id);
    if (tgt_hp.x <= 0) {
        queue_attack_death_event(attacker_id, target_id);
    }
}

void gamestate::resolve_attack_death_event(entityid attacker_id, entityid target_id) {
    if (attacker_id == ENTITYID_INVALID || target_id == ENTITYID_INVALID) {
        return;
    }
    if (get_component_or<DeadFlag>(target_id, false)) {
        return;
    }

    const vec3 target_loc = get_component_or<Position>(target_id, vec3{-1, -1, -1});
    if (!vec3_valid(target_loc)) {
        return;
    }

    tile_t& tile = d.get_floor(target_loc.z)->tile_at(target_loc);
    tile.tile_remove(target_id);
    tile.add_dead_npc(target_id);

    switch ((get_component<EntityTypeTag>(target_id) ? get_component<EntityTypeTag>(target_id)->type : entitytype_t::NONE)) {
    case entitytype_t::NPC:
        queue_attack_award_xp_event(attacker_id, target_id);
        queue_attack_drop_inventory_event(target_id);
        break;
    case entitytype_t::PLAYER: queue_attack_player_death_event(target_id); break;
    default: break;
    }
}

void gamestate::resolve_attack_award_xp_event(entityid attacker_id, entityid target_id) {
    update_npc_xp(attacker_id, target_id);
}

void gamestate::resolve_attack_drop_inventory_event(entityid target_id) {
    drop_all_from_inventory(target_id);
}

void gamestate::resolve_attack_player_death_event([[maybe_unused]] entityid target_id) {
    add_combat_player_death_message();
}

void gamestate::resolve_provoke_npc_event(entityid npc_id, entityid source_id) {
    provoke_npc(npc_id, source_id);
}

void gamestate::resolve_attack_weapon_durability_event(entityid attacker_id, entityid target_id) {
    handle_weapon_durability_loss(attacker_id, target_id);
}

void gamestate::resolve_attack_shield_durability_event(entityid defender_id, entityid attacker_id) {
    handle_shield_durability_loss(defender_id, attacker_id);
}

void gamestate::handle_attack_sfx(entityid attacker, attack_result_t result) {
    vec3 loc = get_component_or<Position>(attacker, vec3{-1, -1, -1});
    massert(vec3_valid(loc), "loc is invalid");
    if (!check_hearing(hero_id, loc)) {
        return;
    }
    const char* index = "sfx/Minifantasy_Dungeon_SFX/07_human_atk_sword_1.wav";
    if (result == attack_result_t::BLOCK) {
        index = "sfx/Minifantasy_Dungeon_SFX/26_sword_hit_3.wav";
    }
    else if (result == attack_result_t::HIT) {
        index = "sfx/Minifantasy_Dungeon_SFX/26_sword_hit_1.wav";
    }
    else if (result == attack_result_t::MISS) {
        entityid weapon_id = get_component_or<EquippedWeapon>(attacker, ENTITYID_INVALID);
        weapontype_t wpn_type = get_component_or<WeaponSubtype>(weapon_id, weapontype_t::NONE);
        index = wpn_type == weapontype_t::SHORT_SWORD ? "sfx/Minifantasy_Dungeon_SFX/07_human_atk_sword_1.wav"
                : wpn_type == weapontype_t::AXE       ? "sfx/Minifantasy_Dungeon_SFX/07_human_atk_sword_1.wav"
                : wpn_type == weapontype_t::DAGGER    ? "sfx/Minifantasy_Dungeon_SFX/07_human_atk_sword_1.wav"
                                                      : "sfx/Minifantasy_Dungeon_SFX/07_human_atk_sword_1.wav";
    }
    audio.queue(index);
    msuccess("attack sfx played");
}

void gamestate::set_gamestate_flag_for_attack_animation(entitytype_t type) {
    massert(type == entitytype_t::PLAYER || type == entitytype_t::NPC, "type is not player or npc!");
    if (type == entitytype_t::PLAYER) {
        flag = gamestate_flag_t::PLAYER_ANIM;
    }
    else if (type == entitytype_t::NPC) {
        flag = gamestate_flag_t::NPC_ANIM;
    }
}

bool gamestate::is_entity_adjacent(entityid id0, entityid id1) {
    massert(id0 != ENTITYID_INVALID, "id0 is invalid");
    massert(id1 != ENTITYID_INVALID, "id1 is invalid");
    massert(id0 != id1, "id0 and id1 are the same");
    vec3 a = get_component_or<Position>(id0, vec3{-1, -1, -1});
    vec3 b = get_component_or<Position>(id1, vec3{-1, -1, -1});
    if (a.z == -1 || b.z == -1 || a.z != b.z) {
        return false;
    }
    return (a.x - 1 == b.x && a.y - 1 == b.y) || (a.x == b.x && a.y - 1 == b.y) || (a.x + 1 == b.x && a.y - 1 == b.y) || (a.x - 1 == b.x && a.y == b.y) ||
           (a.x + 1 == b.x && a.y == b.y) || (a.x - 1 == b.x && a.y + 1 == b.y) || (a.x == b.x && a.y + 1 == b.y) || (a.x + 1 == b.x && a.y + 1 == b.y);
}

void gamestate::update_path_to_target(entityid id) {
    massert(id != INVALID, "id is invalid");
    if (!has_component<NpcPath>(id)) {
        merror("id doesn't have a target_path");
        return;
    }

    auto* path_to_target = get_component<NpcPath>(id);
    path_to_target->value.clear();
    entityid target = get_component_or<TargetEntity>(id, ENTITYID_INVALID);
    if (target == ENTITYID_INVALID) {
        return;
    }

    const Position* start_ptr = get_component<Position>(id);
    const Position* goal_ptr = get_component<Position>(target);
    if (!start_ptr || !goal_ptr) {
        return;
    }

    vec3 start = start_ptr->value;
    vec3 goal = goal_ptr->value;
    if (start.z != goal.z) {
        return;
    }

    shared_ptr<dungeon_floor> df = d.floors[start.z];
    const auto& result = pathfinder.find_path(start, goal, *df, [this](int x, int y, int z) { return tile_has_pushable(x, y, z); });

    path_to_target->value.assign(result.begin(), result.end());
}

bool gamestate::try_entity_move_to_target(entityid id) {
    massert(has_component<NpcPath>(id), "id has no target_path");
    update_path_to_target(id);
    auto* path = get_component<NpcPath>(id);
    if (path->value.size() > 0) {
        massert(has_component<Position>(id), "id has no location");
        vec3 id_location = get_component<Position>(id)->value;
        vec3 next_location = path->value.at(0);
        vec3 diff_location = vec3_sub(next_location, id_location);
        if (run_move_action(id, diff_location)) {
            msuccess2("try entity move succeeded");
            return true;
        }
    }
    return false;
}

bool gamestate::try_entity_move_random(entityid id) {
    uniform_int_distribution<int> dist(-1, 1);
    if (run_move_action(id, vec3{dist(mt), dist(mt), 0})) {
        msuccess2("try entity move succeeded");
        return true;
    }
    return false;
}

bool gamestate::handle_npc(entityid id) {
    minfo2("handle npc %d", id);
    massert(id != ENTITYID_INVALID, "Entity is NULL!");
    if ((get_component<EntityTypeTag>(id) ? get_component<EntityTypeTag>(id)->type : entitytype_t::NONE) != entitytype_t::NPC) {
        return false;
    }
    auto id_name = get_component_or<EntityName>(id, "no-name");
    const DeadFlag* maybe_dead = get_component<DeadFlag>(id);
    massert(maybe_dead != nullptr, "npc id %d name %s has no dead component", id, id_name.c_str());
    if (!maybe_dead) {
        return false;
    }
    bool is_dead = maybe_dead->value;
    if (is_dead) {
        return false;
    }

    entityid tgt_id = get_component_or<TargetEntity>(id, hero_id);
    entity_default_action_t d_action = get_component_or<DefaultAction>(id, entity_default_action_t::NONE);
    if (d_action == entity_default_action_t::NONE) {
        return true;
    }
    else if (d_action == entity_default_action_t::RANDOM_MOVE) {
        if (try_entity_move_random(id)) {
            return true;
        }
    }
    else if (d_action == entity_default_action_t::MOVE_TO_TARGET) {
        if (try_entity_move_to_target(id)) {
            return true;
        }
    }
    else if (d_action == entity_default_action_t::ATTACK_TARGET_IF_ADJACENT) {
        if (is_entity_adjacent(id, tgt_id)) {
            vec3 loc = get_component<Position>(tgt_id)->value;
            run_attack_action(id, loc);
            return true;
        }
    }
    else if (d_action == entity_default_action_t::RANDOM_MOVE_AND_ATTACK_TARGET_IF_ADJACENT) {
        if (is_entity_adjacent(id, tgt_id)) {
            vec3 loc = get_component<Position>(tgt_id)->value;
            run_attack_action(id, loc);
            return true;
        }
        else if (try_entity_move_random(id)) {
            return true;
        }
    }
    else if (d_action == entity_default_action_t::MOVE_TO_TARGET_AND_ATTACK_TARGET_IF_ADJACENT) {
        if (is_entity_adjacent(id, tgt_id)) {
            vec3 loc = get_component<Position>(tgt_id)->value;
            run_attack_action(id, loc);
            return true;
        }
        else if (try_entity_move_to_target(id)) {
            return true;
        }
    }

    merror2("failed to handle npc %d", id);
    return false;
}

void gamestate::handle_npcs() {
    minfo2("handle npcs");
    if (flag == gamestate_flag_t::NPC_TURN) {
#ifndef NPCS_ALL_AT_ONCE
        if (entity_turn >= 1 && entity_turn < next_entityid) {
            if ((get_component<EntityTypeTag>(entity_turn) ? get_component<EntityTypeTag>(entity_turn)->type : entitytype_t::NONE) == entitytype_t::NPC) {
                handle_npc(entity_turn);
                flag = gamestate_flag_t::NPC_ANIM;
            }
            else {
                flag = gamestate_flag_t::NPC_ANIM;
            }
        }
#else
        auto view = registry.view<LegacyEntityId, NpcTag>();
        for (auto entity : view) {
            entityid id = view.get<LegacyEntityId>(entity).id;
            const bool result = handle_npc(id);
            if (result) {
                msuccess2("npc %d handled successfully", id);
            }
            else {
                merror2("npc %d handle failed", id);
            }
        }
        flag = gamestate_flag_t::NPC_ANIM;
#endif
    }
}
