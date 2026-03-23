#pragma once

#include "../pathfinder.h"
#include "../dungeon_floor.h"
#include "../dungeon_tile_type.h"
#include "../entityid.h"
#include <cxxtest/TestSuite.h>
#include <memory>

class PathfinderTestSuite : public CxxTest::TestSuite {
private:
    static const entityid NO_PUSHABLE = ENTITYID_INVALID;

    /** @brief Create an all-walkable floor of the given dimensions. */
    std::shared_ptr<dungeon_floor> make_open_floor(int w, int h, int floor_num = 0) {
        auto df = std::make_shared<dungeon_floor>(w, h);
        df->init(floor_num, biome_t::STONE);
        df->df_set_all_tiles(tiletype_t::FLOOR_STONE_00);
        return df;
    }

    /** @brief Pushable check that always returns no obstacle. */
    static entityid no_pushable(int, int, int) {
        return ENTITYID_INVALID;
    }

public:
    void testOpenGridDiagonalPath() {
        auto df = make_open_floor(8, 8);
        Pathfinder pf;
        const auto& path = pf.find_path(
            vec3{0, 0, 0}, vec3{7, 7, 0}, *df, no_pushable);

        TS_ASSERT(!path.empty());
        // Diagonal path from (0,0) to (7,7) should be 7 steps.
        TS_ASSERT_EQUALS(path.size(), 7u);
        // Final step should be the goal.
        TS_ASSERT(vec3_equal(path.back(), vec3{7, 7, 0}));
        // Each step should be adjacent to the previous.
        vec3 prev{0, 0, 0};
        for (const vec3& step : path) {
            TS_ASSERT(std::abs(step.x - prev.x) <= 1);
            TS_ASSERT(std::abs(step.y - prev.y) <= 1);
            TS_ASSERT(tile_is_walkable(df->tile_at(step).get_type()));
            prev = step;
        }
    }

    void testPathAroundWall() {
        auto df = make_open_floor(12, 12);
        // Place a vertical wall at x=5, except y=6 (the gap).
        for (int y = 0; y < 12; ++y) {
            if (y == 6) continue;
            df->df_set_tile(tiletype_t::STONE_WALL_00, 5, y);
        }

        Pathfinder pf;
        const auto& path = pf.find_path(
            vec3{1, 1, 0}, vec3{9, 1, 0}, *df, no_pushable);

        TS_ASSERT(!path.empty());
        TS_ASSERT(vec3_equal(path.back(), vec3{9, 1, 0}));
        // Verify no step lands on a wall.
        for (const vec3& step : path) {
            TS_ASSERT(tile_is_walkable(df->tile_at(step).get_type()));
        }
    }

    void testNoPathWhenFullyBlocked() {
        auto df = make_open_floor(8, 8);
        // Wall off the entire column x=4 with no gap.
        for (int y = 0; y < 8; ++y) {
            df->df_set_tile(tiletype_t::STONE_WALL_00, 4, y);
        }

        Pathfinder pf;
        const auto& path = pf.find_path(
            vec3{1, 4, 0}, vec3{6, 4, 0}, *df, no_pushable);

        TS_ASSERT(path.empty());
    }

    void testStartEqualsGoal() {
        auto df = make_open_floor(8, 8);
        Pathfinder pf;
        const auto& path = pf.find_path(
            vec3{3, 3, 0}, vec3{3, 3, 0}, *df, no_pushable);

        TS_ASSERT(path.empty());
    }

    void testReuseAcrossMultipleCalls() {
        auto df = make_open_floor(8, 8);
        Pathfinder pf;

        // First call.
        const auto& path1 = pf.find_path(
            vec3{0, 0, 0}, vec3{7, 0, 0}, *df, no_pushable);
        TS_ASSERT(!path1.empty());
        TS_ASSERT(vec3_equal(path1.back(), vec3{7, 0, 0}));

        // Second call with different endpoints — must not be confused by stale data.
        const auto& path2 = pf.find_path(
            vec3{0, 7, 0}, vec3{7, 7, 0}, *df, no_pushable);
        TS_ASSERT(!path2.empty());
        TS_ASSERT(vec3_equal(path2.back(), vec3{7, 7, 0}));

        // Third call — same endpoints as first, verify still correct.
        const auto& path3 = pf.find_path(
            vec3{0, 0, 0}, vec3{7, 0, 0}, *df, no_pushable);
        TS_ASSERT(!path3.empty());
        TS_ASSERT(vec3_equal(path3.back(), vec3{7, 0, 0}));
        TS_ASSERT_EQUALS(path3.size(), 7u);
    }

    void testPushableObstacleAvoided() {
        auto df = make_open_floor(8, 8);
        // Block (4,3) via the pushable callback — forces path around it.
        auto pushable_at_4_3 = [](int x, int y, int) -> entityid {
            return (x == 4 && y == 3) ? static_cast<entityid>(42) : ENTITYID_INVALID;
        };

        Pathfinder pf;
        const auto& path = pf.find_path(
            vec3{3, 3, 0}, vec3{5, 3, 0}, *df, pushable_at_4_3);

        TS_ASSERT(!path.empty());
        TS_ASSERT(vec3_equal(path.back(), vec3{5, 3, 0}));
        // Verify path does not pass through (4,3).
        for (const vec3& step : path) {
            TS_ASSERT(!(step.x == 4 && step.y == 3));
        }
    }

    void testCardinalPathOnNarrowCorridor() {
        auto df = make_open_floor(8, 1);
        Pathfinder pf;
        const auto& path = pf.find_path(
            vec3{0, 0, 0}, vec3{7, 0, 0}, *df, no_pushable);

        TS_ASSERT(!path.empty());
        TS_ASSERT_EQUALS(path.size(), 7u);
        // All steps should be cardinal moves along y=0.
        for (const vec3& step : path) {
            TS_ASSERT_EQUALS(step.y, 0);
        }
    }
};
