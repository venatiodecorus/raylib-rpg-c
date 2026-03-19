/** @file proptype.h
 *  @brief Prop subtype identifiers used for dungeon decoration entities.
 */

#pragma once

/// @brief Supported prop variants that may be spawned into dungeon rooms.
enum class proptype_t
{
    NONE,
    DUNGEON_BANNER_00,
    DUNGEON_BANNER_01,
    DUNGEON_BANNER_02,
    DUNGEON_STATUE_00,
    DUNGEON_TORCH_00,
    DUNGEON_CANDLE_00,
    DUNGEON_JAR_00,
    DUNGEON_PLATE_00,
    DUNGEON_WOODEN_BARREL_OPEN_TOP_EMPTY,
    DUNGEON_WOODEN_BARREL_OPEN_TOP_WATER,
    DUNGEON_WOODEN_CHAIR_00,
    DUNGEON_WOODEN_TABLE_00,
    DUNGEON_WOODEN_TABLE_01,
    DUNGEON_WOODEN_SIGN,
    DUNGEON_COUNT,
};
