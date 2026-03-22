#pragma once

#include "../ecs_actor_components.h"
#include "../ecs_gameplay_components.h"
#include "../gamestate.h"
#include "../inputstate.h"
#include <cxxtest/TestSuite.h>

class HeavySimulationTestSuite : public CxxTest::TestSuite {
private:
    void add_open_floor(gamestate& g, int width = 8, int height = 8) {
        auto df = g.d.create_floor(biome_t::STONE, width, height);
        df->df_set_all_tiles(tiletype_t::FLOOR_STONE_00);
        g.d.add_floor(df);
        g.d.is_initialized = true;
    }

    entityid find_live_npc_on_floor(gamestate& g, int floor, race_t race_value = race_t::NONE) {
        auto view = g.registry.view<LegacyEntityId, NpcTag>();
        for (auto entity : view) {
            entityid id = view.get<LegacyEntityId>(entity).id;
            if (g.get_component_or<DeadFlag>(id, true)) {
                continue;
            }
            if (race_value != race_t::NONE && (g.get_component<ActorKind>(id) ? g.get_component<ActorKind>(id)->race : race_t::NONE) != race_value) {
                continue;
            }
            const vec3 loc = g.get_component_or<Position>(id, vec3{-1, -1, -1});
            if (loc.z == floor) {
                return id;
            }
        }
        return ENTITYID_INVALID;
    }

    vec3 find_open_adjacent_tile(gamestate& g, vec3 origin) {
        auto df = g.d.get_floor(origin.z);
        static constexpr vec3 offsets[] = {
            vec3{0, -1, 0},
            vec3{-1, 0, 0},
            vec3{1, 0, 0},
            vec3{0, 1, 0},
        };

        for (const vec3 offset : offsets) {
            const vec3 loc{origin.x + offset.x, origin.y + offset.y, origin.z};
            if (loc.x < 0 || loc.x >= df->get_width() || loc.y < 0 || loc.y >= df->get_height()) {
                continue;
            }
            tile_t& tile = df->tile_at(loc);
            if (!tile_is_walkable(tile.get_type())) {
                continue;
            }
            if (tile.entity_count() != 0) {
                continue;
            }
            if (tile.get_type() == tiletype_t::UPSTAIRS || tile.get_type() == tiletype_t::DOWNSTAIRS) {
                continue;
            }
            return loc;
        }

        return vec3{-1, -1, -1};
    }

public:
    void testPathfindingSoakMovesHostileOrcThroughSingleGapTowardHero() {
        gamestate g;
        g.test = true;
        add_open_floor(g, 12, 12);
        g.d.current_floor = 0;

        auto df = g.d.get_floor(0);
        for (int y = 0; y < df->get_height(); ++y) {
            if (y == 6) {
                continue;
            }
            df->df_set_tile(tiletype_t::STONE_WALL_00, 5, y);
        }

        const entityid hero = g.create_player_at_with(vec3{9, 6, 0}, "hero", g.player_init(18));
        const entityid orc = g.create_orc_at_with(vec3{1, 6, 0}, [](gamestate&, const entityid) {});

        TS_ASSERT_DIFFERS(hero, ENTITYID_INVALID);
        TS_ASSERT_DIFFERS(orc, ENTITYID_INVALID);

        bool crossed_gap_column = false;
        bool became_adjacent = false;
        vec3 previous_loc = g.get_component_or<Position>(orc, vec3{-1, -1, -1});

        for (int step = 0; step < 32 && !became_adjacent; ++step) {
            g.update_npcs_state();

            auto* path = g.get_component<NpcPath>(orc);
            if (path) {
                path->value.clear();
            }
            g.update_path_to_target(orc);

            auto* refreshed_path = g.get_component<NpcPath>(orc);
            TS_ASSERT(refreshed_path);
            TS_ASSERT(!refreshed_path->value.empty());

            const vec3 next_step = refreshed_path->value.front();
            TS_ASSERT(next_step.x >= 0 && next_step.x < df->get_width());
            TS_ASSERT(next_step.y >= 0 && next_step.y < df->get_height());
            TS_ASSERT(tile_is_walkable(df->tile_at(next_step).get_type()));

            TS_ASSERT(g.handle_npc(orc));

            const vec3 orc_loc = g.get_component_or<Position>(orc, vec3{-1, -1, -1});
            TS_ASSERT(vec3_valid(orc_loc));
            TS_ASSERT(tile_is_walkable(df->tile_at(orc_loc).get_type()));
            TS_ASSERT(!vec3_equal(orc_loc, previous_loc));
            TS_ASSERT(orc_loc.x != 5 || orc_loc.y == 6);

            if (orc_loc.x > 5) {
                crossed_gap_column = true;
            }
            became_adjacent = g.is_entity_adjacent(orc, hero);
            previous_loc = orc_loc;
        }

        TS_ASSERT(crossed_gap_column);
        TS_ASSERT(became_adjacent);
        TS_ASSERT(g.get_component_or<StepsTaken>(orc, 0u) > 0);
    }

    void testTickHeavySimulationResolvesAdjacentOrcFightWithoutBreakingTileState() {
        gamestate g;
        g.test = true;
        g.audio.sfx.resize(71);
        g.random.mt.seed(12345);
        g.logic_init();

        g.d.current_floor = 2;
        const entityid orc = find_live_npc_on_floor(g, 2, race_t::ORC);
        TS_ASSERT_DIFFERS(orc, ENTITYID_INVALID);

        const vec3 orc_loc = g.get_component_or<Position>(orc, vec3{-1, -1, -1});
        const vec3 hero_loc = find_open_adjacent_tile(g, orc_loc);
        TS_ASSERT(vec3_valid(hero_loc));

        g.entity_turn = g.create_player_at_with(hero_loc, "heavy_hero", g.player_init(18));
        TS_ASSERT_DIFFERS(g.hero_id, ENTITYID_INVALID);

        const entityid hero_weapon = g.create_weapon_with(g.sword_init());
        TS_ASSERT_DIFFERS(hero_weapon, ENTITYID_INVALID);
        g.add_to_inventory(g.hero_id, hero_weapon);

        g.make_all_npcs_target_player();
        g.current_scene = scene_t::GAMEPLAY;

        inputstate is;
        inputstate_reset(is);

        bool resolved = false;
        for (int step = 0; step < 256 && !resolved; ++step) {
            g.entity_turn = orc;
            g.flag = gamestate_flag_t::NPC_TURN;
            g.tick(is);

            const bool hero_dead = g.get_component_or<DeadFlag>(g.hero_id, true);
            const bool orc_dead = g.get_component_or<DeadFlag>(orc, true);
            resolved = hero_dead || orc_dead;
        }

        TS_ASSERT(resolved);
        TS_ASSERT_DIFFERS(g.get_component_or<DeadFlag>(g.hero_id, false), g.get_component_or<DeadFlag>(orc, false));

        tile_t& hero_tile = g.d.get_floor(2)->tile_at(hero_loc);
        tile_t& orc_tile = g.d.get_floor(2)->tile_at(orc_loc);
        if (g.get_component_or<DeadFlag>(orc, false)) {
            TS_ASSERT_EQUALS(orc_tile.get_cached_live_npc(), ENTITYID_INVALID);
            TS_ASSERT_EQUALS(orc_tile.get_dead_npc_count(), 1U);
            TS_ASSERT_EQUALS(g.get_component_or<Experience>(g.hero_id, 0), 1);
        } else {
            TS_ASSERT_EQUALS(hero_tile.get_cached_live_npc(), ENTITYID_INVALID);
            TS_ASSERT_EQUALS(hero_tile.get_dead_npc_count(), 1U);
            TS_ASSERT_EQUALS(g.get_component_or<Experience>(orc, 0), 0);
        }
    }
};
