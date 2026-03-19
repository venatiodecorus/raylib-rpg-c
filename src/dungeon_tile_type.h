/** @file dungeon_tile_type.h
 *  @brief Dungeon tile identifiers and tile-category helper functions.
 */

#pragma once


#include <stdbool.h>
#include <string>

/// @brief Enumerated dungeon tile variants used by floor geometry and rendering.
enum class tiletype_t {
    NONE,
    UPSTAIRS,
    DOWNSTAIRS,
    STONE_WALL_00,
    STONE_WALL_01,
    FLOOR_STONE_00,
    FLOOR_STONE_01,
    FLOOR_STONE_02,
    FLOOR_STONE_03,
    FLOOR_STONE_04,
    FLOOR_STONE_05,
    FLOOR_STONE_06,
    FLOOR_STONE_07,
    FLOOR_STONE_08,
    FLOOR_STONE_09,
    FLOOR_STONE_10,
    FLOOR_STONE_11,
    FLOOR_STONE_DIRT_UL_00,
    FLOOR_STONE_DIRT_U_00,
    FLOOR_STONE_DIRT_UR_00,
    FLOOR_STONE_DIRT_L_00,
    FLOOR_STONE_DIRT_C_00,
    FLOOR_STONE_DIRT_R_00,
    FLOOR_STONE_DIRT_DL_00,
    FLOOR_STONE_DIRT_D_00,
    FLOOR_STONE_DIRT_DR_00,
    FLOOR_STONE_DIRT_UL_01,
    FLOOR_STONE_DIRT_U_01,
    FLOOR_STONE_DIRT_UR_01,
    FLOOR_STONE_DIRT_L_01,
    FLOOR_STONE_DIRT_C_01,
    FLOOR_STONE_DIRT_R_01,
    FLOOR_STONE_DIRT_DL_01,
    FLOOR_STONE_DIRT_D_01,
    FLOOR_STONE_DIRT_DR_01,
    FLOOR_STONE_DIRT_UL_02,
    FLOOR_STONE_DIRT_U_02,
    FLOOR_STONE_DIRT_UR_02,
    FLOOR_STONE_DIRT_L_02,
    FLOOR_STONE_DIRT_C_02,
    FLOOR_STONE_DIRT_R_02,
    FLOOR_STONE_DIRT_DL_02,
    FLOOR_STONE_DIRT_D_02,
    FLOOR_STONE_DIRT_DR_02,
    FLOOR_STONE_DIRT_UL_03,
    FLOOR_STONE_DIRT_U_03,
    FLOOR_STONE_DIRT_UR_03,
    FLOOR_STONE_DIRT_DL_03,
    FLOOR_STONE_DIRT_D_03,
    FLOOR_STONE_DIRT_DR_03,
    FLOOR_STONE_DIRT_UL_04,
    FLOOR_STONE_DIRT_U_04,
    FLOOR_STONE_DIRT_UR_04,
    FLOOR_STONE_DIRT_DL_04,
    FLOOR_STONE_DIRT_D_04,
    FLOOR_STONE_DIRT_DR_04,
    FLOOR_STONE_DIRT_UL_05,
    FLOOR_STONE_DIRT_U_05,
    FLOOR_STONE_DIRT_UR_05,
    FLOOR_STONE_DIRT_DL_05,
    FLOOR_STONE_DIRT_D_05,
    FLOOR_STONE_DIRT_DR_05,
    FLOOR_GRASS_00,
    FLOOR_GRASS_01,
    FLOOR_GRASS_02,
    FLOOR_GRASS_03,
    FLOOR_GRASS_04,
    FLOOR_GRASS_05,
    FLOOR_GRASS_06,
    FLOOR_GRASS_07,
    FLOOR_GRASS_08,
    FLOOR_GRASS_09,
    FLOOR_GRASS_10,
    FLOOR_GRASS_11,
    FLOOR_GRASS_12,
    FLOOR_GRASS_13,
    FLOOR_GRASS_14,
    FLOOR_GRASS_15,
    FLOOR_GRASS_16,
    FLOOR_GRASS_17,
    FLOOR_GRASS_18,
    FLOOR_GRASS_19,
    UNKNOWN,
    COUNT
};

/** @brief Convert a tile type into a coarse string label for debugging. */
static inline std::string tiletype2str(const tiletype_t type) {
    if (type == tiletype_t::NONE)
        return "TileNone";
    else if (type == tiletype_t::UNKNOWN)
        return "TileUnknown";
    else if (type == tiletype_t::UPSTAIRS)
        return "TileUpstairs";
    else if (type == tiletype_t::DOWNSTAIRS)
        return "TileDownstairs";
    else if (type >= tiletype_t::FLOOR_STONE_00 && type <= tiletype_t::FLOOR_STONE_11)
        return "TileStone";
    return "TileError";
}

/** @brief Return whether a tile type is the explicit `tiletype_t::NONE` sentinel. */
constexpr static inline bool tiletype_is_none(const tiletype_t type) {
    return type == tiletype_t::NONE;
}

/** @brief Return whether a tile type is not the explicit `tiletype_t::NONE` sentinel. */
constexpr static inline bool tile_is_not_none(const tiletype_t type) {
    return type != tiletype_t::NONE;
}

/** @brief Return whether a tile type is one of the wall tiles. */
constexpr static inline bool tiletype_is_wall(const tiletype_t type) {
    switch (type) {
    case tiletype_t::STONE_WALL_00: return true;
    case tiletype_t::STONE_WALL_01: return true;
    default: break;
    }
    return false;
}

/** @brief Return whether a tile may be walked on by gameplay actors. */
constexpr static inline bool tile_is_walkable(const tiletype_t type) {
    return !tiletype_is_wall(type) && type != tiletype_t::UNKNOWN && type != tiletype_t::NONE && type != tiletype_t::COUNT;
}


/** @brief Return whether a tile type is valid for upstairs placement. */
constexpr static inline bool tile_is_possible_upstairs(const tiletype_t type) {
    return tile_is_walkable(type) && type != tiletype_t::DOWNSTAIRS && type != tiletype_t::COUNT;
}

/** @brief Return whether a tile type is valid for downstairs placement. */
constexpr static inline bool tile_is_possible_downstairs(const tiletype_t type) {
    return tile_is_walkable(type) && type != tiletype_t::UPSTAIRS && type != tiletype_t::COUNT;
}
