#pragma once

#include "../dead_npc_cache.h"
#include "../dungeon_tile.h"
#include "../item_cache.h"
#include <cxxtest/TestSuite.h>

class TileCacheTestSuite : public CxxTest::TestSuite {
public:
    static entityid eid(int v) { return static_cast<entityid>(v); }

    void testDeadNpcCacheBasic() {
        dead_npc_cache cache;
        TS_ASSERT_EQUALS(cache.get_count(), 0U);

        cache.add_id(eid(100));
        TS_ASSERT_EQUALS(cache.get_count(), 1U);
        TS_ASSERT(cache.contains(eid(100)) != -1);

        cache.remove_id(eid(100));
        TS_ASSERT_EQUALS(cache.get_count(), 0U);
        TS_ASSERT_EQUALS(cache.contains(eid(100)), -1);
    }

    void testDeadNpcCacheBoundaries() {
        dead_npc_cache cache;

        for (int i = 0; i < DEAD_NPC_CACHE_SIZE; i++) {
            cache.add_id(eid(i));
            TS_ASSERT_EQUALS(cache.get_count(), static_cast<size_t>(i + 1));
        }

        for (int i = 0; i < DEAD_NPC_CACHE_SIZE; i++) {
            TS_ASSERT(cache.contains(eid(i)) != -1);
        }

        cache.add_id(eid(9));
        TS_ASSERT_EQUALS(cache.get_count(), static_cast<size_t>(DEAD_NPC_CACHE_SIZE));
        TS_ASSERT_EQUALS(cache.contains(eid(0)), -1);
        TS_ASSERT(cache.contains(eid(9)) != -1);
    }

    void testDeadNpcCacheRemove() {
        dead_npc_cache cache;
        cache.add_id(eid(1));
        cache.add_id(eid(2));
        cache.add_id(eid(3));

        TS_ASSERT(cache.remove_id(eid(2)));
        TS_ASSERT_EQUALS(cache.get_count(), 2U);
        TS_ASSERT_EQUALS(cache.contains(eid(2)), -1);
        TS_ASSERT_EQUALS(cache.contains(eid(3)), 1);

        TS_ASSERT(!cache.remove_id(eid(999)));
        TS_ASSERT_EQUALS(cache.get_count(), 2U);
    }

    void testDeadNpcCacheTopAndPop() {
        dead_npc_cache cache;
        TS_ASSERT_EQUALS(cache.top(), INVALID);
        TS_ASSERT_EQUALS(cache.pop(), INVALID);

        cache.add_id(eid(10));
        cache.add_id(eid(20));
        cache.add_id(eid(30));

        TS_ASSERT_EQUALS(cache.top(), eid(30));
        TS_ASSERT_EQUALS(cache.pop(), eid(30));
        TS_ASSERT_EQUALS(cache.pop(), eid(20));
        TS_ASSERT_EQUALS(cache.pop(), eid(10));
        TS_ASSERT_EQUALS(cache.pop(), INVALID);
        TS_ASSERT_EQUALS(cache.get_count(), 0U);
    }

    void testItemCacheBasic() {
        item_cache cache;

        TS_ASSERT_EQUALS(cache.get_count(), 0U);
        TS_ASSERT_EQUALS(cache.top(), INVALID);
        TS_ASSERT_EQUALS(cache.at(0), INVALID);

        TS_ASSERT(cache.add_id(eid(11)));
        TS_ASSERT(cache.add_id(eid(12)));
        TS_ASSERT(cache.add_id(eid(13)));

        TS_ASSERT_EQUALS(cache.get_count(), 3U);
        TS_ASSERT_EQUALS(cache.contains(eid(11)), 0);
        TS_ASSERT_EQUALS(cache.contains(eid(12)), 1);
        TS_ASSERT_EQUALS(cache.contains(eid(13)), 2);
        TS_ASSERT_EQUALS(cache.top(), eid(13));
        TS_ASSERT_EQUALS(cache.at(1), eid(12));
        TS_ASSERT_EQUALS(cache.at(99), INVALID);
    }

    void testItemCacheCapacityAndRemove() {
        item_cache cache;

        for (int i = 0; i < ITEM_CACHE_SIZE; i++) {
            TS_ASSERT(cache.add_id(eid(100 + i)));
        }

        TS_ASSERT_EQUALS(cache.get_count(), static_cast<size_t>(ITEM_CACHE_SIZE));
        TS_ASSERT(!cache.add_id(eid(999)));

        TS_ASSERT(cache.remove_id(eid(103)));
        TS_ASSERT_EQUALS(cache.get_count(), static_cast<size_t>(ITEM_CACHE_SIZE - 1));
        TS_ASSERT_EQUALS(cache.contains(eid(103)), -1);
        TS_ASSERT_EQUALS(cache.contains(eid(104)), 3);

        TS_ASSERT(!cache.remove_id(eid(999)));
    }

    void testTileInitAndCacheReset() {
        tile_t tile(tiletype_t::FLOOR_STONE_00);

        TS_ASSERT_EQUALS(tile.get_type(), tiletype_t::FLOOR_STONE_00);
        TS_ASSERT(!tile.get_visible());
        TS_ASSERT(!tile.get_explored());
        TS_ASSERT(!tile.get_cached_player_present());
        TS_ASSERT_EQUALS(tile.get_cached_live_npc(), ENTITYID_INVALID);
        TS_ASSERT_EQUALS(tile.get_cached_item(), ENTITYID_INVALID);
        TS_ASSERT_EQUALS(tile.get_cached_dead_npc(), ENTITYID_INVALID);
        TS_ASSERT_EQUALS(tile.entity_count(), 0U);
        TS_ASSERT(tile.get_dirty_entities());

        tile.set_visible(true);
        tile.set_explored(true);
        tile.set_cached_player_present(true);
        tile.set_cached_live_npc(eid(50));
        tile.set_cached_item(eid(60));
        tile.set_cached_box(eid(70));
        tile.set_cached_chest(eid(75));
        tile.set_cached_prop(eid(80));
        tile.set_cached_door(eid(90));
        tile.set_dirty_entities(false);
        tile.tile_reset_cache();

        TS_ASSERT_EQUALS(tile.get_type(), tiletype_t::FLOOR_STONE_00);
        TS_ASSERT(tile.get_visible());
        TS_ASSERT(tile.get_explored());
        TS_ASSERT(!tile.get_cached_player_present());
        TS_ASSERT_EQUALS(tile.get_cached_live_npc(), ENTITYID_INVALID);
        TS_ASSERT_EQUALS(tile.get_cached_item(), ENTITYID_INVALID);
        TS_ASSERT_EQUALS(tile.get_cached_box(), ENTITYID_INVALID);
        TS_ASSERT_EQUALS(tile.get_cached_chest(), ENTITYID_INVALID);
        TS_ASSERT_EQUALS(tile.get_cached_prop(), ENTITYID_INVALID);
        TS_ASSERT_EQUALS(tile.get_cached_door(), ENTITYID_INVALID);
        TS_ASSERT_EQUALS(tile.entity_count(), 0U);
        TS_ASSERT(!tile.get_dirty_entities());
    }

    void testTileAddAndRemoveAcrossEntityKinds() {
        tile_t tile(tiletype_t::FLOOR_STONE_00);

        TS_ASSERT_EQUALS(tile.tile_add(eid(1), entitytype_t::PLAYER), eid(1));
        TS_ASSERT(tile.get_cached_player_present());
        TS_ASSERT_EQUALS(tile.get_cached_live_npc(), eid(1));
        TS_ASSERT_EQUALS(tile.entity_count(), 1U);

        TS_ASSERT_EQUALS(tile.tile_remove(eid(1)), eid(1));
        TS_ASSERT(!tile.get_cached_player_present());
        TS_ASSERT_EQUALS(tile.get_cached_live_npc(), ENTITYID_INVALID);

        TS_ASSERT_EQUALS(tile.tile_add(eid(2), entitytype_t::NPC), eid(2));
        TS_ASSERT_EQUALS(tile.tile_add(eid(3), entitytype_t::ITEM), eid(3));
        TS_ASSERT_EQUALS(tile.tile_add(eid(4), entitytype_t::ITEM), eid(4));
        TS_ASSERT_EQUALS(tile.tile_add(eid(5), entitytype_t::PROP), eid(5));
        TS_ASSERT_EQUALS(tile.tile_add(eid(6), entitytype_t::BOX), eid(6));
        TS_ASSERT_EQUALS(tile.tile_add(eid(7), entitytype_t::CHEST), eid(7));
        TS_ASSERT_EQUALS(tile.tile_add(eid(8), entitytype_t::DOOR), eid(8));
        TS_ASSERT(tile.add_dead_npc(eid(9)));

        TS_ASSERT_EQUALS(tile.get_cached_live_npc(), eid(2));
        TS_ASSERT_EQUALS(tile.get_cached_item(), eid(4));
        TS_ASSERT_EQUALS(tile.get_cached_item_at(0), eid(3));
        TS_ASSERT_EQUALS(tile.get_cached_item_at(1), eid(4));
        TS_ASSERT_EQUALS(tile.get_cached_prop(), eid(5));
        TS_ASSERT_EQUALS(tile.get_cached_box(), eid(6));
        TS_ASSERT_EQUALS(tile.get_cached_chest(), eid(7));
        TS_ASSERT_EQUALS(tile.get_cached_door(), eid(8));
        TS_ASSERT_EQUALS(tile.get_cached_dead_npc(), eid(9));
        TS_ASSERT_EQUALS(tile.get_item_count(), 2U);
        TS_ASSERT_EQUALS(tile.get_dead_npc_count(), 1U);
        TS_ASSERT_EQUALS(tile.entity_count(), 8U);

        TS_ASSERT_EQUALS(tile.tile_remove(eid(9)), eid(9));
        TS_ASSERT_EQUALS(tile.get_dead_npc_count(), 0U);
        TS_ASSERT_EQUALS(tile.tile_remove(eid(3)), eid(3));
        TS_ASSERT_EQUALS(tile.get_item_count(), 1U);
        TS_ASSERT_EQUALS(tile.get_cached_item(), eid(4));
        TS_ASSERT_EQUALS(tile.tile_remove(eid(5)), eid(5));
        TS_ASSERT_EQUALS(tile.tile_remove(eid(6)), eid(6));
        TS_ASSERT_EQUALS(tile.tile_remove(eid(7)), eid(7));
        TS_ASSERT_EQUALS(tile.tile_remove(eid(8)), eid(8));
        TS_ASSERT_EQUALS(tile.tile_remove(eid(2)), eid(2));
        TS_ASSERT_EQUALS(tile.entity_count(), 1U);
        TS_ASSERT_EQUALS(tile.tile_remove(eid(4)), eid(4));
        TS_ASSERT_EQUALS(tile.entity_count(), 0U);
        TS_ASSERT_EQUALS(tile.tile_remove(eid(999)), INVALID);
    }

    void testTileItemCacheLimit() {
        tile_t tile(tiletype_t::FLOOR_STONE_00);

        for (int i = 0; i < ITEM_CACHE_SIZE; i++) {
            TS_ASSERT_EQUALS(tile.tile_add(eid(200 + i), entitytype_t::ITEM), eid(200 + i));
        }

        TS_ASSERT_EQUALS(tile.get_item_count(), static_cast<size_t>(ITEM_CACHE_SIZE));
        TS_ASSERT_EQUALS(tile.tile_add(eid(999), entitytype_t::ITEM), INVALID);
        TS_ASSERT_EQUALS(tile.get_item_count(), static_cast<size_t>(ITEM_CACHE_SIZE));
        TS_ASSERT_EQUALS(tile.get_cached_item(), eid(200 + ITEM_CACHE_SIZE - 1));
    }

    void testTileRejectsSecondDoorCacheEntry() {
        tile_t tile(tiletype_t::FLOOR_STONE_00);

        TS_ASSERT_EQUALS(tile.tile_add(eid(10), entitytype_t::DOOR), eid(10));
        TS_ASSERT_EQUALS(tile.tile_add(eid(11), entitytype_t::DOOR), INVALID);
        TS_ASSERT_EQUALS(tile.get_cached_door(), eid(10));
        TS_ASSERT_EQUALS(tile.entity_count(), 1U);
    }

    void testTileSupportsMixedOccupancyAcrossSupportedCacheKinds() {
        tile_t tile(tiletype_t::FLOOR_STONE_00);

        TS_ASSERT_EQUALS(tile.tile_add(eid(10), entitytype_t::PLAYER), eid(10));
        TS_ASSERT_EQUALS(tile.tile_add(eid(20), entitytype_t::ITEM), eid(20));
        TS_ASSERT_EQUALS(tile.tile_add(eid(21), entitytype_t::ITEM), eid(21));
        TS_ASSERT_EQUALS(tile.tile_add(eid(30), entitytype_t::PROP), eid(30));
        TS_ASSERT_EQUALS(tile.tile_add(eid(40), entitytype_t::BOX), eid(40));
        TS_ASSERT_EQUALS(tile.tile_add(eid(45), entitytype_t::CHEST), eid(45));
        TS_ASSERT_EQUALS(tile.tile_add(eid(50), entitytype_t::DOOR), eid(50));
        TS_ASSERT(tile.add_dead_npc(eid(60)));

        TS_ASSERT(tile.get_cached_player_present());
        TS_ASSERT_EQUALS(tile.get_cached_live_npc(), eid(10));
        TS_ASSERT_EQUALS(tile.get_cached_item_at(0), eid(20));
        TS_ASSERT_EQUALS(tile.get_cached_item(), eid(21));
        TS_ASSERT_EQUALS(tile.get_cached_prop(), eid(30));
        TS_ASSERT_EQUALS(tile.get_cached_box(), eid(40));
        TS_ASSERT_EQUALS(tile.get_cached_chest(), eid(45));
        TS_ASSERT_EQUALS(tile.get_cached_door(), eid(50));
        TS_ASSERT_EQUALS(tile.get_cached_dead_npc(), eid(60));
        TS_ASSERT_EQUALS(tile.get_item_count(), 2U);
        TS_ASSERT_EQUALS(tile.get_dead_npc_count(), 1U);
        TS_ASSERT_EQUALS(tile.entity_count(), 8U);
    }

    void testTileMixedOccupancyRemovalPreservesRemainingCaches() {
        tile_t tile(tiletype_t::FLOOR_STONE_00);

        TS_ASSERT_EQUALS(tile.tile_add(eid(10), entitytype_t::PLAYER), eid(10));
        TS_ASSERT_EQUALS(tile.tile_add(eid(20), entitytype_t::ITEM), eid(20));
        TS_ASSERT_EQUALS(tile.tile_add(eid(21), entitytype_t::ITEM), eid(21));
        TS_ASSERT_EQUALS(tile.tile_add(eid(30), entitytype_t::PROP), eid(30));
        TS_ASSERT_EQUALS(tile.tile_add(eid(40), entitytype_t::BOX), eid(40));
        TS_ASSERT_EQUALS(tile.tile_add(eid(45), entitytype_t::CHEST), eid(45));
        TS_ASSERT_EQUALS(tile.tile_add(eid(50), entitytype_t::DOOR), eid(50));
        TS_ASSERT(tile.add_dead_npc(eid(60)));

        TS_ASSERT_EQUALS(tile.tile_remove(eid(50)), eid(50));
        TS_ASSERT_EQUALS(tile.tile_remove(eid(40)), eid(40));
        TS_ASSERT_EQUALS(tile.tile_remove(eid(45)), eid(45));
        TS_ASSERT_EQUALS(tile.tile_remove(eid(60)), eid(60));
        TS_ASSERT_EQUALS(tile.tile_remove(eid(21)), eid(21));

        TS_ASSERT(tile.get_cached_player_present());
        TS_ASSERT_EQUALS(tile.get_cached_live_npc(), eid(10));
        TS_ASSERT_EQUALS(tile.get_cached_item(), eid(20));
        TS_ASSERT_EQUALS(tile.get_cached_prop(), eid(30));
        TS_ASSERT_EQUALS(tile.get_cached_box(), ENTITYID_INVALID);
        TS_ASSERT_EQUALS(tile.get_cached_chest(), ENTITYID_INVALID);
        TS_ASSERT_EQUALS(tile.get_cached_door(), ENTITYID_INVALID);
        TS_ASSERT_EQUALS(tile.get_cached_dead_npc(), ENTITYID_INVALID);
        TS_ASSERT_EQUALS(tile.get_item_count(), 1U);
        TS_ASSERT_EQUALS(tile.get_dead_npc_count(), 0U);
        TS_ASSERT_EQUALS(tile.entity_count(), 3U);
    }

    void testTileTypeHelpers() {
        tile_t wall(tiletype_t::STONE_WALL_00);
        tile_t floor(tiletype_t::FLOOR_STONE_00);

        TS_ASSERT(wall.tile_is_wall());
        TS_ASSERT(!floor.tile_is_wall());
        TS_ASSERT(tile_is_walkable(tiletype_t::FLOOR_STONE_00));
        TS_ASSERT(!tile_is_walkable(tiletype_t::STONE_WALL_00));
        TS_ASSERT(tile_is_possible_upstairs(tiletype_t::FLOOR_STONE_00));
        TS_ASSERT(tile_is_possible_downstairs(tiletype_t::FLOOR_STONE_00));
        TS_ASSERT(!tile_is_possible_upstairs(tiletype_t::DOWNSTAIRS));
        TS_ASSERT(!tile_is_possible_downstairs(tiletype_t::UPSTAIRS));
    }
};
