#pragma once

#include "../gamestate.h"
#include "../ecs_gameplay_components.h"
#include <cxxtest/TestSuite.h>

class EntityPlacementTestSuite : public CxxTest::TestSuite {
private:
    void add_simple_floor(gamestate& g, int width = 5, int height = 5) {
        auto df = g.d.create_floor(biome_t::STONE, width, height);
        df->df_set_all_tiles(tiletype_t::FLOOR_STONE_00);
        g.d.add_floor(df);
    }

    void add_chokepoint_floor(gamestate& g) {
        auto df = g.d.create_floor(biome_t::STONE, 7, 7);
        df->df_set_all_tiles(tiletype_t::NONE);
        df->df_set_tile(tiletype_t::FLOOR_STONE_00, 1, 3);
        df->df_set_tile(tiletype_t::FLOOR_STONE_00, 2, 3);
        df->df_set_tile(tiletype_t::FLOOR_STONE_00, 3, 3);
        df->df_set_tile(tiletype_t::FLOOR_STONE_00, 4, 2);
        df->df_set_tile(tiletype_t::FLOOR_STONE_00, 4, 3);
        df->df_set_tile(tiletype_t::FLOOR_STONE_00, 4, 4);
        df->df_set_tile(tiletype_t::FLOOR_STONE_00, 5, 2);
        df->df_set_tile(tiletype_t::FLOOR_STONE_00, 5, 3);
        df->df_set_tile(tiletype_t::FLOOR_STONE_00, 5, 4);
        g.d.add_floor(df);
    }

public:
    void testPlaceDoorsReturnsZeroOnEmptyDungeon() {
        gamestate g;
        TS_ASSERT_EQUALS(g.place_doors(), 0U);
        TS_ASSERT_EQUALS(g.count_entities_of_type(entitytype_t::DOOR), 0U);
    }

    void testPlaceDoorsCreatesDoorEntitiesWithTileAndComponentState() {
        gamestate g;
        auto floor = g.d.create_floor(biome_t::STONE, 7, 7);
        floor->df_set_all_tiles(tiletype_t::NONE);
        floor->df_set_tile(tiletype_t::FLOOR_STONE_00, 1, 3);
        floor->df_set_tile(tiletype_t::FLOOR_STONE_00, 2, 3);
        floor->df_set_tile(tiletype_t::FLOOR_STONE_00, 3, 3);
        floor->df_set_tile(tiletype_t::FLOOR_STONE_00, 4, 2);
        floor->df_set_tile(tiletype_t::FLOOR_STONE_00, 4, 3);
        floor->df_set_tile(tiletype_t::FLOOR_STONE_00, 4, 4);
        floor->df_set_tile(tiletype_t::FLOOR_STONE_00, 5, 2);
        floor->df_set_tile(tiletype_t::FLOOR_STONE_00, 5, 3);
        floor->df_set_tile(tiletype_t::FLOOR_STONE_00, 5, 4);
        g.d.add_floor(floor);
        floor = g.d.get_floor(0);
        floor->df_refresh_door_candidates();

        const size_t placed = g.place_doors();
        TS_ASSERT(placed > 0);
        TS_ASSERT_EQUALS(g.count_entities_of_type(entitytype_t::DOOR), placed);

        size_t verified = 0;
        g.for_entities_of_type(entitytype_t::DOOR, [&](entityid id) {
            const auto* maybe_loc = g.get_component<Position>(id);
            TS_ASSERT(maybe_loc != nullptr);
            const vec3 loc = maybe_loc->value;
            TS_ASSERT(vec3_valid(loc));

            auto df = g.d.get_floor(static_cast<size_t>(loc.z));
            tile_t& tile = df->tile_at(loc);
            TS_ASSERT_EQUALS(tile.get_cached_door(), id);
            TS_ASSERT(tile_is_walkable(tile.get_type()));
            TS_ASSERT(g.get_component<DoorOpenFlag>(id) != nullptr);
            TS_ASSERT(!g.get_component_or<DoorOpenFlag>(id, true));
            TS_ASSERT(g.get_component_or<NeedsUpdate>(id, false));
            verified++;
        });

        TS_ASSERT_EQUALS(verified, placed);
    }

    void testPlacePropsReturnsZeroOnEmptyDungeon() {
        gamestate g;
        TS_ASSERT_EQUALS(g.place_props(), 0);
        TS_ASSERT_EQUALS(g.count_entities_of_type(entitytype_t::PROP), 0U);
    }

    void testPlacePropsCreatesPropEntitiesWithTileAndComponentState() {
        gamestate g;
        add_simple_floor(g, 6, 6);

        const int placed = g.place_props();
        TS_ASSERT(placed > 0);
        TS_ASSERT_EQUALS(g.count_entities_of_type(entitytype_t::PROP), static_cast<size_t>(placed));

        size_t verified = 0;
        g.for_entities_of_type(entitytype_t::PROP, [&](entityid id) {
            const auto* maybe_loc = g.get_component<Position>(id);
            TS_ASSERT(maybe_loc != nullptr);
            const vec3 loc = maybe_loc->value;
            TS_ASSERT(vec3_valid(loc));

            auto df = g.d.get_floor(static_cast<size_t>(loc.z));
            tile_t& tile = df->tile_at(loc);
            TS_ASSERT_EQUALS(tile.get_cached_prop(), id);
            TS_ASSERT(tile_is_walkable(tile.get_type()));
            TS_ASSERT(tile.get_type() != tiletype_t::UPSTAIRS);
            TS_ASSERT(tile.get_type() != tiletype_t::DOWNSTAIRS);
            TS_ASSERT(!tile.get_can_have_door());
            TS_ASSERT(g.get_component_or<PropTypeComponent>(id, proptype_t::NONE) != proptype_t::NONE);
            TS_ASSERT(g.get_component<EntityName>(id) != nullptr);
            TS_ASSERT(g.get_component<EntityDescription>(id) != nullptr);
            TS_ASSERT(g.get_component<SolidTag>(id) != nullptr);
            TS_ASSERT(g.get_component<PushableTag>(id) != nullptr);
            TS_ASSERT(g.get_component_or<NeedsUpdate>(id, false));
            verified++;
        });

        TS_ASSERT_EQUALS(verified, static_cast<size_t>(placed));
    }

    void testPlacePropsSkipsChokepointAndDoorApproachLayouts() {
        gamestate g;
        add_chokepoint_floor(g);

        const int placed = g.place_props();
        TS_ASSERT_EQUALS(placed, 0);
        TS_ASSERT_EQUALS(g.count_entities_of_type(entitytype_t::PROP), 0U);
    }

    void testPlacePropsSkipsFourthFloorTutorialLevel() {
        gamestate g;
        add_simple_floor(g, 6, 6);
        add_simple_floor(g, 6, 6);
        add_simple_floor(g, 6, 6);
        add_simple_floor(g, 6, 6);

        const int placed = g.place_props();
        TS_ASSERT(placed > 0);

        size_t tutorial_floor_props = 0;
        g.for_entities_of_type(entitytype_t::PROP, [&](entityid id) {
            const vec3 loc = g.get_component_or<Position>(id, vec3{-1, -1, -1});
            TS_ASSERT(vec3_valid(loc));
            if (loc.z == 2) {
                tutorial_floor_props++;
            }
        });

        TS_ASSERT_EQUALS(tutorial_floor_props, 0U);
    }

    void testGetRandomLocSkipsOccupiedChestTile() {
        gamestate g;
        add_simple_floor(g, 4, 4);

        auto df = g.d.get_floor(0);
        const vec3 blocked_loc{1, 1, 0};
        const entityid chest_id = g.create_chest_at_with(blocked_loc, [](gamestate&, const entityid) {});
        TS_ASSERT_DIFFERS(chest_id, ENTITYID_INVALID);

        const vec3 random_loc = df->get_random_loc();
        TS_ASSERT(vec3_valid(random_loc));
        TS_ASSERT(!vec3_equal(random_loc, blocked_loc));
    }
};
