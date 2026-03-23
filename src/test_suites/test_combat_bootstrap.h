#pragma once

#include "../ecs_gameplay_components.h"
#include "../gamestate.h"
#include "../inputstate.h"
#include <cxxtest/TestSuite.h>
#include <set>

class CombatBootstrapTestSuite : public CxxTest::TestSuite {
private:
    void add_floor(gamestate& g, int width = 8, int height = 8) {
        auto df = g.d.create_floor(biome_t::STONE, width, height);
        df->df_set_all_tiles(tiletype_t::FLOOR_STONE_00);
        g.d.add_floor(df);
        g.d.is_initialized = true;
    }

public:
    void testCreateMultipleOrcsProducesUniqueIdsAndNpcState() {
        gamestate g;
        add_floor(g, 10, 10);

        std::set<entityid> ids;
        const vec3 locs[] = {{1, 1, 0}, {2, 1, 0}, {3, 1, 0}, {4, 1, 0}};
        for (const vec3 loc : locs) {
            const entityid id = g.create_orc_at_with(loc, [](gamestate&, const entityid) { });
            TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
            ids.insert(id);
            TS_ASSERT_EQUALS((g.get_component<EntityTypeTag>(id) ? g.get_component<EntityTypeTag>(id)->type : entitytype_t::NONE), entitytype_t::NPC);
            TS_ASSERT_EQUALS((g.get_component<ActorKind>(id) ? g.get_component<ActorKind>(id)->race : race_t::NONE), race_t::ORC);
            TS_ASSERT(!g.get_component_or<DeadFlag>(id, true));
        }

        TS_ASSERT_EQUALS(ids.size(), 4U);
        {
            size_t npc_count = 0;
            for (auto e : g.registry.view<NpcTag>()) {
                if (!g.get_component_or<DeadFlag>(e, true) && g.get_component_or<Position>(e, vec3{-1, -1, -1}).z == 0)
                    npc_count++;
            }
            TS_ASSERT_EQUALS(npc_count, 4U);
        }
    }

    void testCreateOrcRejectsOccupiedTile() {
        gamestate g;
        add_floor(g, 8, 8);

        const vec3 loc{1, 1, 0};
        const entityid first = g.create_orc_at_with(loc, [](gamestate&, const entityid) { });
        const entityid second = g.create_orc_at_with(loc, [](gamestate&, const entityid) { });

        TS_ASSERT_DIFFERS(first, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(second, ENTITYID_INVALID);
        {
            size_t npc_count = 0;
            for (auto e : g.registry.view<NpcTag>()) {
                if (!g.get_component_or<DeadFlag>(e, true) && g.get_component_or<Position>(e, vec3{-1, -1, -1}).z == 0)
                    npc_count++;
            }
            TS_ASSERT_EQUALS(npc_count, 1U);
        }
    }

    void testCreateOrcStopsAtWalkableTileCapacityWithoutLeakingIds() {
        gamestate g;
        add_floor(g, 3, 3);

        std::set<entityid> ids;
        for (int y = 0; y < 3; ++y) {
            for (int x = 0; x < 3; ++x) {
                const entityid id = g.create_orc_at_with(vec3{x, y, 0}, [](gamestate&, const entityid) { });
                TS_ASSERT_DIFFERS(id, ENTITYID_INVALID);
                ids.insert(id);
            }
        }

        TS_ASSERT_EQUALS(ids.size(), 9U);
        {
            size_t npc_count = 0;
            for (auto e : g.registry.view<NpcTag>()) {
                if (!g.get_component_or<DeadFlag>(e, true) && g.get_component_or<Position>(e, vec3{-1, -1, -1}).z == 0)
                    npc_count++;
            }
            TS_ASSERT_EQUALS(npc_count, 9U);
        }

        const entityid before_next_entity = g.next_entityid;
        const entityid extra = g.create_orc_at_with(vec3{1, 1, 0}, [](gamestate&, const entityid) { });

        TS_ASSERT_EQUALS(extra, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(g.next_entityid, before_next_entity);
        {
            size_t npc_count = 0;
            for (auto e : g.registry.view<NpcTag>()) {
                if (!g.get_component_or<DeadFlag>(e, true) && g.get_component_or<Position>(e, vec3{-1, -1, -1}).z == 0)
                    npc_count++;
            }
            TS_ASSERT_EQUALS(npc_count, 9U);
        }
    }

    void testLogicInitBuildsGameplayBootstrap() {
        gamestate g;
        g.test = true;
        g.mt.seed(12345);

        g.logic_init();

        TS_ASSERT(g.d.is_initialized);
        TS_ASSERT_EQUALS(g.d.get_floor_count(), 4U);
        TS_ASSERT_EQUALS(g.d.current_floor, 0);
        TS_ASSERT_EQUALS(g.d.get_floor(0)->get_width(), 8);
        TS_ASSERT_EQUALS(g.d.get_floor(0)->get_height(), 8);
        TS_ASSERT_EQUALS(g.d.get_floor(1)->get_width(), 24);
        TS_ASSERT_EQUALS(g.d.get_floor(1)->get_height(), 24);
        TS_ASSERT_EQUALS(g.d.get_floor(2)->get_width(), 16);
        TS_ASSERT_EQUALS(g.d.get_floor(2)->get_height(), 16);
        TS_ASSERT_EQUALS(g.d.get_floor(3)->get_width(), 16);
        TS_ASSERT_EQUALS(g.d.get_floor(3)->get_height(), 16);
        TS_ASSERT(vec3_valid(g.d.get_floor(0)->get_upstairs_loc()));
        TS_ASSERT(vec3_valid(g.d.get_floor(0)->get_downstairs_loc()));
        TS_ASSERT(g.count_entities_of_type(entitytype_t::DOOR) >= 1U);
        TS_ASSERT(g.count_entities_of_type(entitytype_t::BOX) >= 4U);
        TS_ASSERT(g.count_entities_of_type(entitytype_t::ITEM) >= 2U);
        {
            auto count_floor = [&](int floor) {
                size_t count = 0;
                for (auto e : g.registry.view<NpcTag>()) {
                    if (!g.get_component_or<DeadFlag>(e, true) && g.get_component_or<Position>(e, vec3{-1, -1, -1}).z == floor)
                        count++;
                }
                return count;
            };
            TS_ASSERT(count_floor(0) >= 1U);
            TS_ASSERT(count_floor(1) >= 9U);
            TS_ASSERT(count_floor(2) >= 1U);
        }
        TS_ASSERT(g.messages.system.size() >= 2U);
    }

    void testLogicInitPlacesFriendlyNpcGreenSlimesAndArmedOrc() {
        gamestate g;
        g.test = true;
        g.mt.seed(12345);

        g.logic_init();

        auto find_live_on_floor = [&](int floor) -> entityid {
            entityid result = ENTITYID_INVALID;
            for (auto e : g.registry.view<NpcTag>()) {
                if (result != ENTITYID_INVALID)
                    break;
                if (g.get_component_or<DeadFlag>(e, true))
                    continue;
                if (g.get_component_or<Position>(e, vec3{-1, -1, -1}).z == floor)
                    result = e;
            }
            return result;
        };

        const entityid floor_zero_npc = find_live_on_floor(0);
        const entityid floor_one_npc = find_live_on_floor(1);
        const entityid floor_two_npc = find_live_on_floor(2);

        TS_ASSERT_DIFFERS(floor_zero_npc, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(floor_one_npc, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(floor_two_npc, ENTITYID_INVALID);
        TS_ASSERT(!g.get_component_or<AggroFlag>(floor_zero_npc, true));
        TS_ASSERT_EQUALS((g.get_component<ActorKind>(floor_one_npc) ? g.get_component<ActorKind>(floor_one_npc)->race : race_t::NONE), race_t::GREEN_SLIME);
        TS_ASSERT(!g.get_component_or<AggroFlag>(floor_one_npc, true));
        {
            size_t slime_count = 0;
            for (auto e : g.registry.view<NpcTag>()) {
                if (!g.get_component_or<DeadFlag>(e, true) &&
                    (g.get_component<ActorKind>(e) ? g.get_component<ActorKind>(e)->race : race_t::NONE) == race_t::GREEN_SLIME &&
                    g.get_component_or<Position>(e, vec3{-1, -1, -1}).z == 1)
                    slime_count++;
            }
            TS_ASSERT_EQUALS(slime_count, 9U);
        }
        TS_ASSERT_EQUALS((g.get_component<ActorKind>(floor_two_npc) ? g.get_component<ActorKind>(floor_two_npc)->race : race_t::NONE), race_t::ORC);
        TS_ASSERT(g.get_component_or<AggroFlag>(floor_two_npc, false));
        const auto* npc_inv = g.get_component<Inventory>(floor_two_npc);
        TS_ASSERT(npc_inv);
        TS_ASSERT_EQUALS(npc_inv->value.size(), 2U);
        const entityid equipped_weapon_id = g.get_component_or<EquippedWeapon>(floor_two_npc, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(equipped_weapon_id, ENTITYID_INVALID);
        TS_ASSERT_EQUALS(
            (g.get_component<ItemKind>(equipped_weapon_id) ? g.get_component<ItemKind>(equipped_weapon_id)->type : itemtype_t::NONE), itemtype_t::WEAPON);
        bool found_potion = false;
        for (entityid item_id : npc_inv->value) {
            if ((g.get_component<ItemKind>(item_id) ? g.get_component<ItemKind>(item_id)->type : itemtype_t::NONE) == itemtype_t::POTION &&
                (g.get_component<PotionKind>(item_id) ? g.get_component<PotionKind>(item_id)->type : potiontype_t::NONE) == potiontype_t::HP_SMALL) {
                found_potion = true;
            }
        }
        TS_ASSERT(found_potion);
    }

    void testUpdateNpcsStateSetsFriendlyAndHostileDefaultActions() {
        gamestate g;
        add_floor(g, 8, 8);
        add_floor(g, 8, 8);

        const entityid friendly = g.create_npc_at_with(race_t::DWARF, vec3{1, 1, 0}, [](gamestate&, const entityid) { });
        const entityid hostile = g.create_orc_at_with(vec3{2, 2, 1}, [](gamestate&, const entityid) { });

        TS_ASSERT_DIFFERS(friendly, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(hostile, ENTITYID_INVALID);

        g.update_npcs_state();

        TS_ASSERT_EQUALS(g.get_component_or<DefaultAction>(friendly, entity_default_action_t::NONE), entity_default_action_t::RANDOM_MOVE);
        TS_ASSERT_EQUALS(g.get_component_or<DefaultAction>(hostile, entity_default_action_t::NONE), entity_default_action_t::RANDOM_MOVE);
        TS_ASSERT_EQUALS(g.get_component_or<TargetEntity>(hostile, ENTITYID_INVALID), ENTITYID_INVALID);

        g.d.current_floor = 1;
        g.hero_id = g.create_player_at_with(vec3{4, 4, 1}, "hero", g.player_init(10));
        g.update_npcs_state();

        TS_ASSERT_EQUALS(g.get_component_or<TargetEntity>(hostile, ENTITYID_INVALID), g.hero_id);
        TS_ASSERT_EQUALS(
            g.get_component_or<DefaultAction>(hostile, entity_default_action_t::NONE), entity_default_action_t::MOVE_TO_TARGET_AND_ATTACK_TARGET_IF_ADJACENT);

        g.hero_id = g.create_player_at_with(vec3{3, 2, 1}, "hero_adjacent", g.player_init(10));
        g.update_npcs_state();

        TS_ASSERT_EQUALS(g.get_component_or<DefaultAction>(hostile, entity_default_action_t::NONE), entity_default_action_t::ATTACK_TARGET_IF_ADJACENT);
    }

    void testProvokeNpcTurnsFriendlyNpcHostile() {
        gamestate g;
        add_floor(g, 8, 8);

        const entityid friendly = g.create_npc_at_with(race_t::DWARF, vec3{2, 1, 0}, [](gamestate&, const entityid) { });
        const entityid hero = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(10));

        TS_ASSERT_DIFFERS(friendly, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(hero, ENTITYID_INVALID);
        TS_ASSERT(!g.get_component_or<AggroFlag>(friendly, true));

        g.provoke_npc(friendly, hero);

        TS_ASSERT(g.get_component_or<AggroFlag>(friendly, false));
        TS_ASSERT_EQUALS(g.get_component_or<TargetEntity>(friendly, ENTITYID_INVALID), hero);
        TS_ASSERT_EQUALS(g.get_component_or<DefaultAction>(friendly, entity_default_action_t::NONE), entity_default_action_t::ATTACK_TARGET_IF_ADJACENT);
    }

    void testAttackingFriendlyNpcSetsAggro() {
        gamestate g;
        g.test = true;
        add_floor(g, 8, 8);

        const entityid friendly = g.create_npc_at_with(race_t::DWARF, vec3{2, 1, 0}, [](gamestate&, const entityid) { });
        const entityid hero = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(10));

        TS_ASSERT_DIFFERS(friendly, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(hero, ENTITYID_INVALID);
        TS_ASSERT(!g.get_component_or<AggroFlag>(friendly, true));

        g.run_attack_action(hero, vec3{2, 1, 0});

        TS_ASSERT(g.get_component_or<AggroFlag>(friendly, false));
        TS_ASSERT_EQUALS(g.get_component_or<TargetEntity>(friendly, ENTITYID_INVALID), hero);
    }

    void testBoundedMeleeDuelEndsWithConsistentDeathState() {
        gamestate g;
        g.test = true;
        g.mt.seed(24680);
        add_floor(g, 8, 8);

        const entityid hero = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(12));
        const entityid orc = g.create_orc_at_with(vec3{2, 1, 0}, [](gamestate&, const entityid) { });

        TS_ASSERT_DIFFERS(hero, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(orc, ENTITYID_INVALID);

        const entityid hero_weapon = g.create_weapon_with(g.sword_init());
        const entityid orc_weapon = g.create_weapon_with(g.sword_init());
        TS_ASSERT_DIFFERS(hero_weapon, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(orc_weapon, ENTITYID_INVALID);

        g.add_to_inventory(hero, hero_weapon);
        g.add_to_inventory(orc, orc_weapon);


        bool duel_finished = false;
        for (int round = 0; round < 16 && !duel_finished; ++round) {
            if (!g.get_component_or<DeadFlag>(hero, true) && !g.get_component_or<DeadFlag>(orc, true)) {
                g.run_attack_action(hero, vec3{2, 1, 0});
            }
            if (!g.get_component_or<DeadFlag>(hero, true) && !g.get_component_or<DeadFlag>(orc, true)) {
                g.run_attack_action(orc, vec3{1, 1, 0});
            }
            duel_finished = g.get_component_or<DeadFlag>(hero, false) || g.get_component_or<DeadFlag>(orc, false);
        }

        TS_ASSERT(duel_finished);
        TS_ASSERT_DIFFERS(g.get_component_or<DeadFlag>(hero, false), g.get_component_or<DeadFlag>(orc, false));

        const vec2 hero_hp = g.get_component_or<HitPoints>(hero, vec2{-1, -1});
        const vec2 orc_hp = g.get_component_or<HitPoints>(orc, vec2{-1, -1});
        TS_ASSERT(hero_hp.x <= hero_hp.y);
        TS_ASSERT(orc_hp.x <= orc_hp.y);

        tile_t& hero_tile = g.d.get_floor(0)->tile_at(vec3{1, 1, 0});
        tile_t& orc_tile = g.d.get_floor(0)->tile_at(vec3{2, 1, 0});
        if (g.get_component_or<DeadFlag>(orc, false)) {
            TS_ASSERT_EQUALS(hero_tile.get_dead_npc_count(), 0U);
            TS_ASSERT_EQUALS(orc_tile.get_dead_npc_count(), 1U);
            TS_ASSERT_EQUALS(g.get_component_or<Experience>(hero, 0), 1);
        }
        else {
            TS_ASSERT_EQUALS(hero_tile.get_dead_npc_count(), 1U);
            TS_ASSERT_EQUALS(orc_tile.get_dead_npc_count(), 0U);
            TS_ASSERT_EQUALS(g.get_component_or<Experience>(orc, 0), 0);
        }
    }

    void testRunAttackActionUsesQueuedAttackIntent() {
        gamestate g;
        g.test = true;
        g.mt.seed(24680);
        add_floor(g, 8, 8);

        const entityid hero = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(12));
        const entityid orc = g.create_orc_at_with(vec3{2, 1, 0}, [](gamestate&, const entityid) { });
        TS_ASSERT_DIFFERS(hero, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(orc, ENTITYID_INVALID);

        const entityid hero_weapon = g.create_weapon_with(g.sword_init());
        TS_ASSERT_DIFFERS(hero_weapon, ENTITYID_INVALID);
        g.add_to_inventory(hero, hero_weapon);


        const attack_result_t result = g.run_attack_action(hero, vec3{2, 1, 0});

        TS_ASSERT_DIFFERS(result, attack_result_t::NONE);
        TS_ASSERT(g.gameplay_events.empty());
        TS_ASSERT_EQUALS(g.get_component_or<Facing>(hero, direction_t::NONE), direction_t::RIGHT);
    }

    void testRunAttackActionProvokesFriendlyNpcThroughQueuedFollowup() {
        gamestate g;
        g.test = true;
        g.mt.seed(24680);
        add_floor(g, 8, 8);

        const entityid friendly = g.create_npc_at_with(race_t::DWARF, vec3{2, 1, 0}, [](gamestate&, const entityid) { });
        const entityid hero = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(12));
        const entityid hero_weapon = g.create_weapon_with(g.sword_init());
        TS_ASSERT_DIFFERS(friendly, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(hero, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(hero_weapon, ENTITYID_INVALID);

        g.add_to_inventory(hero, hero_weapon);

        TS_ASSERT(!g.get_component_or<AggroFlag>(friendly, true));

        g.run_attack_action(hero, vec3{2, 1, 0});

        TS_ASSERT(g.get_component_or<AggroFlag>(friendly, false));
        TS_ASSERT_EQUALS(g.get_component_or<TargetEntity>(friendly, ENTITYID_INVALID), hero);
        TS_ASSERT(g.gameplay_events.empty());
    }

    void testRunAttackActionKillsTargetThroughQueuedAttackIntent() {
        gamestate g;
        g.test = true;
        g.mt.seed(7);
        add_floor(g, 8, 8);

        const entityid hero = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(12));
        const entityid orc = g.create_orc_at_with(vec3{2, 1, 0}, [](gamestate&, const entityid) { });
        TS_ASSERT_DIFFERS(hero, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(orc, ENTITYID_INVALID);

        const entityid hero_weapon = g.create_weapon_with(g.sword_init());
        TS_ASSERT_DIFFERS(hero_weapon, ENTITYID_INVALID);
        g.add_to_inventory(hero, hero_weapon);

        const attack_result_t result = g.run_attack_action(hero, vec3{2, 1, 0});

        TS_ASSERT_EQUALS(result, attack_result_t::HIT);
        TS_ASSERT(g.get_component_or<DeadFlag>(orc, false));
        tile_t& target_tile = g.d.get_floor(0)->tile_at(vec3{2, 1, 0});
        TS_ASSERT_EQUALS(target_tile.get_cached_live_npc(), ENTITYID_INVALID);
        TS_ASSERT_EQUALS(target_tile.get_cached_dead_npc(), orc);
        TS_ASSERT_EQUALS(g.get_component_or<Experience>(hero, 0), 1);
        TS_ASSERT(g.gameplay_events.empty());
    }

    void testRunAttackActionDropsNpcInventoryThroughQueuedDeathFollowups() {
        gamestate g;
        g.test = true;
        g.mt.seed(7);
        add_floor(g, 8, 8);

        const entityid hero = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(12));
        const entityid orc = g.create_orc_at_with(vec3{2, 1, 0}, [](gamestate&, const entityid) { });
        const entityid hero_weapon = g.create_weapon_with(g.sword_init());
        const entityid loot = g.create_weapon_at_with(vec3{2, 1, 0}, g.dagger_init());
        TS_ASSERT_DIFFERS(hero, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(orc, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(hero_weapon, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(loot, ENTITYID_INVALID);

        g.add_to_inventory(hero, hero_weapon);
        g.add_to_inventory(orc, loot);

        const attack_result_t result = g.run_attack_action(hero, vec3{2, 1, 0});

        TS_ASSERT_EQUALS(result, attack_result_t::HIT);
        TS_ASSERT(g.get_component_or<DeadFlag>(orc, false));
        TS_ASSERT(!g.is_in_inventory(orc, loot));
        TS_ASSERT(vec3_equal(g.get_component_or<Position>(loot, vec3{-1, -1, -1}), vec3{2, 1, 0}));
        tile_t& target_tile = g.d.get_floor(0)->tile_at(vec3{2, 1, 0});
        TS_ASSERT_EQUALS(target_tile.get_cached_item(), loot);
        TS_ASSERT(g.gameplay_events.empty());
    }

    void testRunAttackActionResolvesQueuedShieldBlockFollowup() {
        gamestate g;
        g.test = true;
        g.mt.seed(99);
        add_floor(g, 8, 8);

        const entityid hero = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(12));
        const entityid orc = g.create_orc_at_with(vec3{2, 1, 0}, [](gamestate&, const entityid) { });
        TS_ASSERT_DIFFERS(hero, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(orc, ENTITYID_INVALID);

        const entityid hero_weapon = g.create_weapon_with(g.sword_init());
        const entityid shield = g.create_shield_with(g.shield_init());
        TS_ASSERT_DIFFERS(hero_weapon, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(shield, ENTITYID_INVALID);

        g.add_to_inventory(hero, hero_weapon);
        g.add_to_inventory(orc, shield);


        const attack_result_t result = g.run_attack_action(hero, vec3{2, 1, 0});

        TS_ASSERT_EQUALS(result, attack_result_t::BLOCK);
        const vec2 orc_hp = g.get_component_or<HitPoints>(orc, vec2{-1, -1});
        TS_ASSERT_EQUALS(orc_hp.x, 12);
        TS_ASSERT_EQUALS(orc_hp.y, 12);
        TS_ASSERT(g.get_component_or<BlockSuccessFlag>(orc, false));
        TS_ASSERT(g.gameplay_events.empty());
    }

    void testRunAttackActionNonBreakBlockMessageSequenceIsStable() {
        gamestate g;
        g.test = true;
        g.mt.seed(99);
        add_floor(g, 8, 8);

        const entityid hero = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(12));
        const entityid hero_weapon = g.create_weapon_with(g.sword_init());
        const entityid shield = g.create_shield_with(g.shield_init());
        TS_ASSERT_DIFFERS(hero, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(orc, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(hero_weapon, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(shield, ENTITYID_INVALID);

        g.add_to_inventory(hero, hero_weapon);
        g.add_to_inventory(orc, shield);

        const size_t initial_history_size = g.messages.history.size();
        const attack_result_t result = g.run_attack_action(hero, vec3{2, 1, 0});

        TS_ASSERT_EQUALS(result, attack_result_t::BLOCK);
        TS_ASSERT_EQUALS(g.messages.history.size(), initial_history_size + 1);
        TS_ASSERT(g.messages.history.back().find("blocked an attack") != string::npos);
        TS_ASSERT(g.messages.history.back().find("broke") == string::npos);
        TS_ASSERT(g.messages.history.back().find("deals") == string::npos);
        TS_ASSERT(g.gameplay_events.empty());
    }

    void testRunAttackActionNonLethalHitMessageSequenceIsStable() {
        gamestate g;
        g.test = true;
        g.mt.seed(7);
        add_floor(g, 8, 8);

        const entityid hero = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(12));
        const entityid hero_weapon = g.create_weapon_with(g.sword_init());
        TS_ASSERT_DIFFERS(hero, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(orc, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(hero_weapon, ENTITYID_INVALID);

        g.add_to_inventory(hero, hero_weapon);

        const size_t initial_history_size = g.messages.history.size();
        const attack_result_t result = g.run_attack_action(hero, vec3{2, 1, 0});

        TS_ASSERT_EQUALS(result, attack_result_t::HIT);
        TS_ASSERT_EQUALS(g.messages.history.size(), initial_history_size + 1);
        TS_ASSERT(g.messages.history.back().find("deals") != string::npos);
        TS_ASSERT(g.messages.history.back().find("broke") == string::npos);
        TS_ASSERT(g.messages.history.back().find("blocked") == string::npos);
        TS_ASSERT(!g.messages.is_active);
        TS_ASSERT(g.gameplay_events.empty());
    }

    void testRunAttackActionResolvesQueuedWeaponDurabilityBreakFollowup() {
        gamestate g;
        g.test = true;
        g.mt.seed(7);
        add_floor(g, 8, 8);

        const entityid hero = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(12));
        const entityid orc = g.create_orc_at_with(vec3{2, 1, 0}, [](gamestate&, const entityid) { });
        const entityid hero_weapon = g.create_weapon_with(g.sword_init());
        TS_ASSERT_DIFFERS(hero, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(orc, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(hero_weapon, ENTITYID_INVALID);

        g.add_to_inventory(hero, hero_weapon);
        const size_t initial_history_size = g.messages.history.size();

        const attack_result_t result = g.run_attack_action(hero, vec3{2, 1, 0});

        TS_ASSERT_EQUALS(result, attack_result_t::HIT);
        TS_ASSERT_EQUALS(g.get_component_or<EquippedWeapon>(hero, ENTITYID_INVALID), ENTITYID_INVALID);
        TS_ASSERT(g.get_component_or<DestroyedFlag>(hero_weapon, false));
        TS_ASSERT(!g.is_in_inventory(hero, hero_weapon));
        TS_ASSERT_EQUALS(g.messages.history.size(), initial_history_size + 2);
        TS_ASSERT(g.messages.history[initial_history_size].find("deals") != string::npos);
        TS_ASSERT(g.messages.history[initial_history_size + 1].find("broke!") != string::npos);
        TS_ASSERT(g.gameplay_events.empty());
    }

    void testRunAttackActionResolvesQueuedShieldDurabilityBreakFollowup() {
        gamestate g;
        g.test = true;
        g.mt.seed(99);
        add_floor(g, 8, 8);

        const entityid hero = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(12));
        const entityid orc = g.create_orc_at_with(vec3{2, 1, 0}, [](gamestate&, const entityid) { });
        const entityid hero_weapon = g.create_weapon_with(g.sword_init());
        const entityid shield = g.create_shield_with(g.shield_init());
        TS_ASSERT_DIFFERS(hero, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(orc, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(hero_weapon, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(shield, ENTITYID_INVALID);

        g.add_to_inventory(hero, hero_weapon);
        g.add_to_inventory(orc, shield);
        const size_t initial_history_size = g.messages.history.size();

        const attack_result_t result = g.run_attack_action(hero, vec3{2, 1, 0});

        TS_ASSERT_EQUALS(result, attack_result_t::BLOCK);
        TS_ASSERT_EQUALS(g.get_component_or<EquippedShield>(orc, ENTITYID_INVALID), ENTITYID_INVALID);
        TS_ASSERT(g.get_component_or<DestroyedFlag>(shield, false));
        TS_ASSERT(!g.is_in_inventory(orc, shield));
        TS_ASSERT_EQUALS(g.messages.history.size(), initial_history_size + 2);
        TS_ASSERT(g.messages.history[initial_history_size].find("blocked an attack") != string::npos);
        TS_ASSERT(g.messages.history[initial_history_size + 1].find("broke!") != string::npos);
        TS_ASSERT(g.gameplay_events.empty());
    }

    void testRunAttackActionMissesWithoutDamageDeathOrDurabilitySideEffects() {
        gamestate g;
        g.test = true;
        g.mt.seed(7);
        add_floor(g, 8, 8);

        const entityid hero = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(12));
        const entityid hero_weapon = g.create_weapon_with(g.sword_init());
        const entityid shield = g.create_shield_with(g.shield_init());
        TS_ASSERT_DIFFERS(hero, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(orc, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(hero_weapon, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(shield, ENTITYID_INVALID);

        g.add_to_inventory(hero, hero_weapon);
        g.add_to_inventory(orc, shield);

        const size_t initial_history_size = g.messages.history.size();
        const attack_result_t result = g.run_attack_action(hero, vec3{2, 1, 0});

        TS_ASSERT_EQUALS(result, attack_result_t::MISS);
        TS_ASSERT(!g.get_component_or<DamagedFlag>(orc, false));
        TS_ASSERT(!g.get_component_or<DeadFlag>(orc, false));
        TS_ASSERT(!g.get_component_or<BlockSuccessFlag>(orc, false));
        TS_ASSERT_EQUALS(g.get_component_or<HitPoints>(orc, vec2{-1, -1}).x, 12);
        TS_ASSERT_EQUALS(g.get_component_or<ItemDurability>(hero_weapon, -1), 5);
        TS_ASSERT_EQUALS(g.get_component_or<ItemDurability>(shield, -1), 5);
        TS_ASSERT(g.get_component_or<AggroFlag>(orc, false));
        TS_ASSERT_EQUALS(g.get_component_or<TargetEntity>(orc, ENTITYID_INVALID), hero);
        TS_ASSERT(g.damage_popups_sys.popups.empty());
        TS_ASSERT_EQUALS(g.get_component_or<Experience>(hero, 0), 0);
        TS_ASSERT_EQUALS(g.messages.history.size(), initial_history_size + 1);
        TS_ASSERT(g.messages.history.back().find("misses!") != string::npos);
        TS_ASSERT(g.messages.history.back().find("deals") == string::npos);
        TS_ASSERT(g.messages.history.back().find("blocked") == string::npos);
        TS_ASSERT(g.messages.history.back().find("broke") == string::npos);
        TS_ASSERT(g.gameplay_events.empty());
    }

    void testRunAttackActionLethalNpcHitMessageSequenceIsStable() {
        gamestate g;
        g.test = true;
        g.mt.seed(7);
        add_floor(g, 8, 8);

        const entityid hero = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(12));
        const entityid hero_weapon = g.create_weapon_with(g.sword_init());
        TS_ASSERT_DIFFERS(hero, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(orc, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(hero_weapon, ENTITYID_INVALID);

        g.add_to_inventory(hero, hero_weapon);

        const size_t initial_history_size = g.messages.history.size();
        const attack_result_t result = g.run_attack_action(hero, vec3{2, 1, 0});

        TS_ASSERT_EQUALS(result, attack_result_t::HIT);
        TS_ASSERT(g.get_component_or<DeadFlag>(orc, false));
        TS_ASSERT_EQUALS(g.messages.history.size(), initial_history_size + 1);
        TS_ASSERT(g.messages.history.back().find("deals") != string::npos);
        TS_ASSERT(g.messages.history.back().find("broke") == string::npos);
        TS_ASSERT(g.messages.history.back().find("blocked") == string::npos);
        TS_ASSERT_EQUALS(g.get_component_or<Experience>(hero, 0), 1);
        TS_ASSERT(!g.messages.is_active);
        TS_ASSERT(g.gameplay_events.empty());
    }

    void testRunAttackActionLethalHitOnHeroResolvesQueuedPlayerDeathFollowup() {
        gamestate g;
        g.test = true;
        g.mt.seed(7);
        add_floor(g, 8, 8);

        const entityid hero = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(12));
        const entityid orc_weapon = g.create_weapon_with(g.sword_init());
        TS_ASSERT_DIFFERS(hero, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(orc, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(orc_weapon, ENTITYID_INVALID);

        g.hero_id = hero;
        g.add_to_inventory(orc, orc_weapon);

        tile_t& hero_tile = g.d.get_floor(0)->tile_at(vec3{1, 1, 0});
        const size_t initial_history_size = g.messages.history.size();
        const attack_result_t result = g.run_attack_action(orc, vec3{1, 1, 0});

        TS_ASSERT_EQUALS(result, attack_result_t::HIT);
        TS_ASSERT(g.get_component_or<DeadFlag>(hero, false));
        TS_ASSERT(g.get_component_or<PullableTag>(hero, false));
        TS_ASSERT_EQUALS(hero_tile.get_cached_live_npc(), ENTITYID_INVALID);
        TS_ASSERT_EQUALS(hero_tile.get_cached_dead_npc(), hero);
        TS_ASSERT_EQUALS(g.damage_popups_sys.popups.size(), 1U);
        TS_ASSERT_EQUALS(g.messages.history.size(), initial_history_size + 1);
        TS_ASSERT(g.messages.history.back().find("deals") != string::npos);
        TS_ASSERT(g.messages.is_active);
        TS_ASSERT(!g.messages.system.empty());
        TS_ASSERT_EQUALS(g.messages.system.front(), "You died");
        TS_ASSERT(g.gameplay_events.empty());
    }

    void testTickInTestModeAdvancesTicksAndTurnsWithHeroPresent() {
        gamestate g;
        g.test = true;
        g.audio.sfx.resize(71);
        g.logic_init();

        const vec3 hero_loc = g.d.get_floor(0)->get_random_loc();
        TS_ASSERT(vec3_valid(hero_loc));

        const int maxhp_roll = 10;
        g.create_player_at_with(hero_loc, "darkmage", g.player_init(maxhp_roll));
        TS_ASSERT_DIFFERS(g.hero_id, ENTITYID_INVALID);

        g.make_all_npcs_target_player();
        g.current_scene = scene_t::GAMEPLAY;

        inputstate is;
        inputstate_reset(is);

        const unsigned int before_turns = g.turn_count;
        const unsigned long before_ticks = g.ticks;
        const int num_ticks = static_cast<int>(g.next_entityid) * 2;
        for (int i = 0; i < num_ticks; i++) {
            g.tick(is);
        }

        TS_ASSERT(g.ticks >= before_ticks + static_cast<unsigned long>(num_ticks));
        TS_ASSERT(g.turn_count > before_turns);
        TS_ASSERT(g.has_component<DeadFlag>(g.hero_id));
        TS_ASSERT(!g.get_component_or<DeadFlag>(g.hero_id, true));
    }

    void testResolveAttackDamageEventAddsDamagePopupForHpTarget() {
        gamestate g;
        add_floor(g, 8, 8);

        const entityid attacker = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(10));
        const entityid target = g.create_orc_at_with(vec3{2, 1, 0}, [](gamestate&, const entityid) { });
        TS_ASSERT_DIFFERS(attacker, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(target, ENTITYID_INVALID);

        g.resolve_attack_damage_event(attacker, target, 3);

        TS_ASSERT_EQUALS(g.damage_popups_sys.popups.size(), 1U);
        TS_ASSERT_EQUALS(g.damage_popups_sys.popups[0].amount, 3);
        TS_ASSERT_EQUALS(g.damage_popups_sys.popups[0].floor, 0);
        TS_ASSERT(g.frame_dirty);
    }

    void testUpdateDamagePopupsExpiresFinishedEntries() {
        gamestate g;
        add_floor(g, 8, 8);

        const entityid target = g.create_orc_at_with(vec3{2, 2, 0}, [](gamestate&, const entityid) { });
        TS_ASSERT_DIFFERS(target, ENTITYID_INVALID);

        {
            const vec3 loc = *g.get_component<Position>(target)->value;
            g.damage_popups_sys.add(loc.x, loc.y, loc.z, 3, false, g.mt);
        }
        TS_ASSERT_EQUALS(g.damage_popups_sys.popups.size(), 1U);

        g.damage_popups_sys.update(0.35f);
        TS_ASSERT_EQUALS(g.damage_popups_sys.popups.size(), 1U);

        g.damage_popups_sys.update(0.40f);
        TS_ASSERT(g.damage_popups_sys.popups.empty());
        TS_ASSERT(g.frame_dirty);
    }
};
