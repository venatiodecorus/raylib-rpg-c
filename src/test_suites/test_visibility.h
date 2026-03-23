#pragma once

#include "../calculate_linear_path.h"
#include "../ecs_gameplay_components.h"
#include "../gamestate.h"
#include <cxxtest/TestSuite.h>

class VisibilityTestSuite : public CxxTest::TestSuite {
private:
    void add_floor(gamestate& g, int w = 12, int h = 12) {
        auto df = g.d.create_floor(biome_t::STONE, w, h);
        df->df_set_all_tiles(tiletype_t::FLOOR_STONE_00);
        g.d.add_floor(df);
        g.d.is_initialized = true;
    }

public:
    // ── calculate_path_with_thickness tests ──────────────────────────

    void testHorizontalPathNoThickness() {
        vector<vec3> path = calculate_path_with_thickness(vec3{0, 0, 0}, vec3{5, 0, 0});
        TS_ASSERT_EQUALS(path.size(), 5u);
        for (const vec3& p : path) {
            TS_ASSERT_EQUALS(p.y, 0);
        }
    }

    void testVerticalPathNoThickness() {
        vector<vec3> path = calculate_path_with_thickness(vec3{0, 0, 0}, vec3{0, 5, 0});
        TS_ASSERT_EQUALS(path.size(), 5u);
        for (const vec3& p : path) {
            TS_ASSERT_EQUALS(p.x, 0);
        }
    }

    void testDiagonalPathHasThickness() {
        vector<vec3> path = calculate_path_with_thickness(vec3{0, 0, 0}, vec3{4, 4, 0});
        // A pure diagonal of 4 steps would yield 4 tiles; thickness adds neighbors.
        TS_ASSERT(path.size() > 4u);
    }

    void testSamePointEmptyPath() {
        vector<vec3> path = calculate_path_with_thickness(vec3{3, 3, 0}, vec3{3, 3, 0});
        TS_ASSERT(path.empty());
    }

    void testAdjacentTileSingleStep() {
        vector<vec3> path = calculate_path_with_thickness(vec3{0, 0, 0}, vec3{1, 0, 0});
        TS_ASSERT_EQUALS(path.size(), 1u);
    }

    // ── path_blocked tests ───────────────────────────────────────────

    void testPathBlockedByWall() {
        gamestate g;
        add_floor(g);
        auto df = g.d.get_floor(0);
        // Place a wall between start (1,5,0) and end (10,5,0).
        df->df_set_tile(tiletype_t::STONE_WALL_00, 5, 5);

        TS_ASSERT(g.path_blocked(vec3{1, 5, 0}, vec3{10, 5, 0}));
    }

    void testPathNotBlockedOnOpenFloor() {
        gamestate g;
        add_floor(g);

        TS_ASSERT(!g.path_blocked(vec3{1, 1, 0}, vec3{10, 1, 0}));
    }

    void testPathBlockedByClosedDoor() {
        gamestate g;
        add_floor(g);
        // Create a closed door at (5,1,0) between start and end.
        entityid door = g.create_door_at_with(vec3{5, 1, 0}, [](gamestate&, const entityid) {});
        TS_ASSERT_DIFFERS(door, ENTITYID_INVALID);

        TS_ASSERT(g.path_blocked(vec3{1, 1, 0}, vec3{10, 1, 0}));
    }

    // ── visibility_path_blocked tests ────────────────────────────────

    void testVisibilityNotBlockedByDoorAtDestination() {
        gamestate g;
        add_floor(g);
        // Place a closed door at the destination tile.
        entityid door = g.create_door_at_with(vec3{5, 1, 0}, [](gamestate&, const entityid) {});
        TS_ASSERT_DIFFERS(door, ENTITYID_INVALID);

        // Visibility should NOT be blocked when the closed door is at the destination.
        TS_ASSERT(!g.visibility_path_blocked(vec3{1, 1, 0}, vec3{5, 1, 0}));
    }

    void testVisibilityBlockedByDoorInMiddle() {
        gamestate g;
        add_floor(g);
        // Place a closed door in the middle of the path.
        entityid door = g.create_door_at_with(vec3{5, 1, 0}, [](gamestate&, const entityid) {});
        TS_ASSERT_DIFFERS(door, ENTITYID_INVALID);

        // Visibility IS blocked when the closed door is not at the destination.
        TS_ASSERT(g.visibility_path_blocked(vec3{1, 1, 0}, vec3{10, 1, 0}));
    }
};
