#pragma once

#include "../ecs_actor_components.h"
#include "../ecs_gameplay_components.h"
#include "../gamestate.h"
#include <cxxtest/TestSuite.h>

class CombatMathTestSuite : public CxxTest::TestSuite {
private:
    void add_floor(gamestate& g, int width = 8, int height = 8) {
        auto df = g.d.create_floor(biome_t::STONE, width, height);
        df->df_set_all_tiles(tiletype_t::FLOOR_STONE_00);
        g.d.add_floor(df);
        g.d.is_initialized = true;
    }

public:
    void testComputeArmorClassDefaultIsTen() {
        gamestate g;
        add_floor(g);

        const entityid npc = g.create_npc_at_with(race_t::HUMAN, vec3{1, 1, 0}, [](gamestate&, const entityid) {});
        TS_ASSERT_DIFFERS(npc, ENTITYID_INVALID);

        int ac = g.compute_armor_class(npc);
        TS_ASSERT_EQUALS(ac, 10);
    }

    void testComputeArmorClassUsesBaseAndDexBonus() {
        gamestate g;
        add_floor(g);

        const entityid npc = g.create_npc_at_with(race_t::HUMAN, vec3{1, 1, 0}, [](gamestate& gs, const entityid id) {
            gs.registry.emplace_or_replace<BaseArmorClass>(id, BaseArmorClass{12});
            gs.registry.emplace_or_replace<DexterityAttr>(id, DexterityAttr{14});
        });
        TS_ASSERT_DIFFERS(npc, ENTITYID_INVALID);

        int ac = g.compute_armor_class(npc);
        // base 12 + dex bonus for 14 = (14-10)/2 = 2, total = 14
        TS_ASSERT_EQUALS(ac, 14);
    }

    void testComputeAttackRollHitsWhenRollMeetsAc() {
        gamestate g;
        add_floor(g);

        const entityid attacker = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(10));
        const entityid target = g.create_npc_at_with(race_t::HUMAN, vec3{2, 1, 0}, [](gamestate&, const entityid) {});
        TS_ASSERT_DIFFERS(attacker, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(target, ENTITYID_INVALID);

        // Give the attacker high STR for a big bonus
        g.registry.emplace_or_replace<StrengthAttr>(attacker, StrengthAttr{20});
        // Seed for a high roll
        g.mt.seed(42);

        bool hit = g.compute_attack_roll(attacker, target);
        TS_ASSERT(hit);
    }

    void testComputeAttackRollMissesWhenRollBelowAc() {
        gamestate g;
        add_floor(g);

        const entityid attacker = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(10));
        const entityid target = g.create_npc_at_with(race_t::HUMAN, vec3{2, 1, 0}, [](gamestate& gs, const entityid id) {
            gs.registry.emplace_or_replace<BaseArmorClass>(id, BaseArmorClass{30});
        });
        TS_ASSERT_DIFFERS(attacker, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(target, ENTITYID_INVALID);

        // Low STR gives negative bonus
        g.registry.emplace_or_replace<StrengthAttr>(attacker, StrengthAttr{3});
        g.mt.seed(1);

        bool hit = g.compute_attack_roll(attacker, target);
        TS_ASSERT(!hit);
    }

    void testComputeAttackDamageMinimumIsOne() {
        gamestate g;
        add_floor(g);

        const entityid attacker = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(10));
        const entityid target = g.create_npc_at_with(race_t::HUMAN, vec3{2, 1, 0}, [](gamestate&, const entityid) {});
        TS_ASSERT_DIFFERS(attacker, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(target, ENTITYID_INVALID);

        // No equipped weapon => uses MINIMUM_DAMAGE {1,1,0}
        // Low STR so bonus is 0 (max(0, bonus))
        g.registry.emplace_or_replace<StrengthAttr>(attacker, StrengthAttr{10});
        g.mt.seed(42);

        int dmg = g.compute_attack_damage(attacker, target);
        TS_ASSERT(dmg >= 1);
    }

    void testComputeAttackDamageUsesWeaponDice() {
        gamestate g;
        add_floor(g);

        const entityid attacker = g.create_player_at_with(vec3{1, 1, 0}, "hero", g.player_init(10));
        const entityid target = g.create_npc_at_with(race_t::HUMAN, vec3{2, 1, 0}, [](gamestate&, const entityid) {});
        TS_ASSERT_DIFFERS(attacker, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(target, ENTITYID_INVALID);

        const entityid sword = g.create_weapon_with(g.sword_init());
        TS_ASSERT_DIFFERS(sword, ENTITYID_INVALID);
        g.add_to_inventory(attacker, sword);

        // High STR for positive bonus
        g.registry.emplace_or_replace<StrengthAttr>(attacker, StrengthAttr{16});
        g.mt.seed(42);

        int dmg = g.compute_attack_damage(attacker, target);
        // STR 16 => bonus = (16-10)/2 = 3, weapon dice roll >= 1, so damage >= 4
        TS_ASSERT(dmg >= 4);
    }

    void testIsEntityAdjacentAllEightDirections() {
        gamestate g;
        add_floor(g);

        const entityid center = g.create_npc_at_with(race_t::HUMAN, vec3{4, 4, 0}, [](gamestate&, const entityid) {});
        TS_ASSERT_DIFFERS(center, ENTITYID_INVALID);

        // Test all 8 adjacent positions around (4,4)
        const vec3 dirs[] = {
            {3, 3, 0}, {4, 3, 0}, {5, 3, 0},
            {3, 4, 0},            {5, 4, 0},
            {3, 5, 0}, {4, 5, 0}, {5, 5, 0}
        };

        for (const vec3& pos : dirs) {
            const entityid neighbor = g.create_npc_at_with(race_t::HUMAN, pos, [](gamestate&, const entityid) {});
            TS_ASSERT_DIFFERS(neighbor, ENTITYID_INVALID);
            TS_ASSERT(g.is_entity_adjacent(center, neighbor));
            // Clean up: remove from tile so we can reuse positions
            auto df = g.d.get_floor(0);
            tile_t& t = df->tile_at(pos);
            t.tile_remove(neighbor);
            g.registry.destroy(neighbor);
        }
    }

    void testIsEntityAdjacentDifferentFloorsFalse() {
        gamestate g;
        // Create two floors
        auto df0 = g.d.create_floor(biome_t::STONE, 8, 8);
        df0->df_set_all_tiles(tiletype_t::FLOOR_STONE_00);
        g.d.add_floor(df0);
        auto df1 = g.d.create_floor(biome_t::STONE, 8, 8);
        df1->df_set_all_tiles(tiletype_t::FLOOR_STONE_00);
        g.d.add_floor(df1);
        g.d.is_initialized = true;

        const entityid a = g.create_npc_at_with(race_t::HUMAN, vec3{4, 4, 0}, [](gamestate&, const entityid) {});
        const entityid b = g.create_npc_at_with(race_t::HUMAN, vec3{4, 5, 1}, [](gamestate&, const entityid) {});
        TS_ASSERT_DIFFERS(a, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(b, ENTITYID_INVALID);

        TS_ASSERT(!g.is_entity_adjacent(a, b));
    }

    void testIsEntityAdjacentSameTileFalse() {
        gamestate g;
        add_floor(g);

        // Create two entities manually at the same position
        const entityid a = g.create_npc_at_with(race_t::HUMAN, vec3{4, 4, 0}, [](gamestate&, const entityid) {});
        TS_ASSERT_DIFFERS(a, ENTITYID_INVALID);

        // Create a second entity and manually set its position to the same tile
        const entityid b = g.add_entity();
        g.registry.emplace<EntityTypeTag>(b, EntityTypeTag{entitytype_t::NPC});
        g.registry.emplace<NpcTag>(b, NpcTag{});
        g.registry.emplace<Position>(b, Position{vec3{4, 4, 0}});
        g.registry.emplace<DeadFlag>(b, DeadFlag{false});

        // Same tile: is_entity_adjacent checks 8 surrounding tiles, not same tile
        // The function asserts id0 != id1 and they're at the same position,
        // so none of the 8-direction checks will match
        TS_ASSERT(!g.is_entity_adjacent(a, b));
    }
};
