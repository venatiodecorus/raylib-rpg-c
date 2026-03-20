#pragma once
/**
 * @file texture_defs.h
 * @brief Compile-time texture definitions for all 501 game textures.
 *
 * Replaces the old runtime-parsed textures.txt with a static array of
 * texture_def structs.  Each entry specifies how to load/generate a texture
 * and its sprite-sheet geometry (contexts x frames).
 */

#include <cstddef>      // NULL
#include "texture_ids.h"

/* ------------------------------------------------------------------ */
/*  Source-type enum                                                    */
/* ------------------------------------------------------------------ */

/** @brief Source type for a texture definition. */
enum texture_src_type {
    TXSRC_FILE,        ///< Load entire PNG file
    TXSRC_TILEMAP,     ///< Crop sub-rectangle from tilemap PNG
    TXSRC_PLACEHOLDER  ///< Generate magenta placeholder
};

/* ------------------------------------------------------------------ */
/*  Per-entry struct                                                    */
/* ------------------------------------------------------------------ */

/** @brief Compile-time definition for one texture entry. */
struct texture_def {
    int txkey;              ///< Index into txinfo[] array (matches TX_* defines)
    int contexts;           ///< Number of sprite rows (directions)
    int frames;             ///< Number of sprite columns (animation frames per row)
    bool dither;            ///< Apply dithering on load
    texture_src_type src_type;
    const char* path;       ///< File path (NULL for PLACEHOLDER)
    int src_x, src_y;       ///< Tilemap source origin (TILEMAP only)
    int src_w, src_h;       ///< Source dimensions (TILEMAP: crop size; PLACEHOLDER: image size; FILE: ignored)
};

/* ------------------------------------------------------------------ */
/*  Helper macros                                                      */
/* ------------------------------------------------------------------ */

#define TX_FILE(key, ctx, fr, filepath) \
    {key, ctx, fr, false, TXSRC_FILE, filepath, 0, 0, 0, 0}

#define TX_PLACEHOLDER(key, ctx, fr) \
    {key, ctx, fr, false, TXSRC_PLACEHOLDER, NULL, 0, 0, (fr)*32, (ctx)*32}

#define TX_TILEMAP(key, ctx, fr, filepath, sx, sy, sw, sh) \
    {key, ctx, fr, false, TXSRC_TILEMAP, filepath, sx, sy, sw, sh}

/* ------------------------------------------------------------------ */
/*  Asset base-path shorthands (keep definitions readable)             */
/* ------------------------------------------------------------------ */

#define GFX_CREATURES  "gfx/Minifantasy_Creatures_v3.3_Free_Version/Minifantasy_Creatures_Assets/"
#define GFX_HUMAN      GFX_CREATURES "Base_Humanoids/Human/Base_Human/"
#define GFX_HUMAN_SH   GFX_HUMAN "_Shadows/"
#define GFX_ORC        GFX_CREATURES "Base_Humanoids/Orc/Base_Orc/"
#define GFX_ORC_SH     GFX_ORC "_Shadows/"
#define GFX_ELF        GFX_CREATURES "Base_Humanoids/Elf/"
#define GFX_ELF_SH     GFX_ELF "_Shadows/"
#define GFX_DWARF      GFX_CREATURES "Base_Humanoids/Dwarf/Base_Dwarf/"
#define GFX_DWARF_SH   GFX_DWARF "_Shadows/"
#define GFX_DWARF2     GFX_CREATURES "Base_Humanoids/Dwarf/Dwarf_Yellow_Beard/"
#define GFX_DWARF2_SH  GFX_DWARF2 "_Shadows/"
#define GFX_HALFLING   GFX_CREATURES "Base_Humanoids/Halfling/"
#define GFX_HALFLING_SH GFX_HALFLING "_Shadows/"
#define GFX_GOBLIN     GFX_CREATURES "Base_Humanoids/Goblin/"
#define GFX_GOBLIN_SH  GFX_GOBLIN "_Shadows/"
#define GFX_WOLF       GFX_CREATURES "Beasts/Wolf/"
#define GFX_WOLF_SH    GFX_WOLF "_Shadows/"
#define GFX_BAT        GFX_CREATURES "Beasts/Bat/"
#define GFX_BAT_SH     GFX_BAT "_Shadows/"
#define GFX_WARG       GFX_CREATURES "Beasts/Warg/"
#define GFX_WARG_SH    GFX_WARG "_Shadows/"
#define GFX_GSLIME     GFX_CREATURES "Slimes/Green_Slime/"
#define GFX_GSLIME_SH  GFX_GSLIME "_Shadows/"
#define GFX_SKELETON   GFX_CREATURES "Undead/Skeleton/"
#define GFX_SKELETON_SH GFX_SKELETON "_Shadows/"
#define GFX_ZOMBIE     GFX_CREATURES "Undead/Zombie/"
#define GFX_ZOMBIE_SH  GFX_ZOMBIE "_Shadows/"

#define GFX_DUNGEON_TS "gfx/Minifantasy_Dungeon_v2.3_Free_Version/Minifantasy_Dungeon_Assets/Tileset/Tileset.png"

/* ------------------------------------------------------------------ */
/*  Master texture definitions array                                   */
/* ------------------------------------------------------------------ */

#define TEXTURE_DEF_COUNT 501

static const texture_def TEXTURE_DEFS[TEXTURE_DEF_COUNT] = {

    /* =============================================================
     *  DOORS  (IDs 0-3) — no source files, 32x32 placeholder
     * ============================================================= */
    TX_PLACEHOLDER(TX_DOOR_WOODEN_OPEN_01,   1, 1),  //   0
    TX_PLACEHOLDER(TX_DOOR_WOODEN_OPEN_00,   1, 1),  //   1
    TX_PLACEHOLDER(TX_DOOR_WOODEN_CLOSED_01, 1, 1),  //   2
    TX_PLACEHOLDER(TX_DOOR_WOODEN_CLOSED_00, 1, 1),  //   3

    /* =============================================================
     *  TILES  (IDs 4-166) — 32x32 placeholders (dungeon tileset is
     *  8x8 which doesn't match the game's 32x32 grid)
     * ============================================================= */
    TX_PLACEHOLDER(TX_TILES_GRASS_15,                1, 1),  //   4
    TX_PLACEHOLDER(TX_TILES_STONE_GRASS_08,          1, 1),  //   5
    TX_PLACEHOLDER(TX_TILES_GRASS_12,                1, 1),  //   6
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_00,            1, 1),  //   7
    TX_TILEMAP(TX_TILES_UPSTAIRS_00,             1, 1, GFX_DUNGEON_TS, 136, 40, 8, 8),  //   8
    TX_TILEMAP(TX_TILES_STONE_10,                1, 1, GFX_DUNGEON_TS, 136, 24, 8, 8),  //   9
    TX_PLACEHOLDER(TX_TILES_STONE_GRASS_06,          1, 1),  //  10
    TX_PLACEHOLDER(TX_TILES_DIRT_GRASS_11,           1, 1),  //  11
    TX_PLACEHOLDER(TX_TILES_STONE_GRASS_01,          1, 1),  //  12
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_DL_05,        1, 1),  //  13
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_STONE_11,      1, 1),  //  14
    TX_TILEMAP(TX_TILES_STONE_TRAP_ON_00,        1, 1, GFX_DUNGEON_TS, 104, 88, 8, 8),  //  15
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_DR_03,        1, 1),  //  16
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_GRASS_13,      1, 1),  //  17
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_L_02,         1, 1),  //  18
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_DR_04,        1, 1),  //  19
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_GRASS_14,      1, 1),  //  20
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_DL_02,        1, 1),  //  21
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_STONE_16,      1, 1),  //  22
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_U_01,         1, 1),  //  23
    TX_PLACEHOLDER(TX_TILES_DIRT_05,                 1, 1),  //  24
    TX_PLACEHOLDER(TX_TILES_DIRT_02,                 1, 1),  //  25
    TX_PLACEHOLDER(TX_TILES_TOWN_WALL_STONE_00,      1, 1),  //  26
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_UR_05,        1, 1),  //  27
    TX_PLACEHOLDER(TX_TILES_WATER_01,                1, 1),  //  28
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_UL_03,        1, 1),  //  29
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_UL_04,        1, 1),  //  30
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_STONE_09,      1, 1),  //  31
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_UR_02,        1, 1),  //  32
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_GRASS_05,      1, 1),  //  33
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_STONE_07,      1, 1),  //  34
    TX_TILEMAP(TX_TILES_DOWNSTAIRS_00,           1, 1, GFX_DUNGEON_TS, 136, 40, 8, 8),  //  35
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_STONE_00,      1, 1),  //  36
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_GRASS_02,      1, 1),  //  37
    TX_TILEMAP(TX_TILES_STONE_08,                1, 1, GFX_DUNGEON_TS, 120, 24, 8, 8),  //  38
    TX_TILEMAP(TX_TILES_STONE_TRAP_OFF_00,       1, 1, GFX_DUNGEON_TS, 144, 24, 8, 8),  //  39
    TX_PLACEHOLDER(TX_TILES_DIRT_GRASS_07,           1, 1),  //  40
    TX_PLACEHOLDER(TX_TILES_TOWN_INTERIOR_FLOOR_01,  1, 1),  //  41
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_D_00,         1, 1),  //  42
    TX_PLACEHOLDER(TX_TILES_STONE_GRASS_10,          1, 1),  //  43
    TX_PLACEHOLDER(TX_TILES_DIRT_GRASS_00,           1, 1),  //  44
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_C_02,         1, 1),  //  45
    TX_PLACEHOLDER(TX_TILES_GRASS_03,                1, 1),  //  46
    TX_TILEMAP(TX_TILES_STONE_01,                1, 1, GFX_DUNGEON_TS, 112, 16, 8, 8),  //  47
    TX_TILEMAP(TX_TILES_STONE_06,                1, 1, GFX_DUNGEON_TS, 104, 24, 8, 8),  //  48
    TX_PLACEHOLDER(TX_TILES_DIRT_GRASS_09,           1, 1),  //  49
    TX_PLACEHOLDER(TX_TILES_GRASS_04,                1, 1),  //  50
    TX_PLACEHOLDER(TX_TILES_DIRT_03,                 1, 1),  //  51
    TX_PLACEHOLDER(TX_TILES_DIRT_04,                 1, 1),  //  52
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_U_00,         1, 1),  //  53
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_DR_05,        1, 1),  //  54
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_STONE_17,      1, 1),  //  55
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_DL_03,        1, 1),  //  56
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_STONE_10,      1, 1),  //  57
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_DL_04,        1, 1),  //  58
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_R_02,         1, 1),  //  59
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_DR_02,        1, 1),  //  60
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_GRASS_12,      1, 1),  //  61
    TX_PLACEHOLDER(TX_TILES_STONE_GRASS_00,          1, 1),  //  62
    TX_PLACEHOLDER(TX_TILES_STONE_GRASS_07,          1, 1),  //  63
    TX_PLACEHOLDER(TX_TILES_DIRT_GRASS_10,           1, 1),  //  64
    TX_PLACEHOLDER(TX_TILES_STONE_GRASS_09,          1, 1),  //  65
    TX_PLACEHOLDER(TX_TILES_GRASS_13,                1, 1),  //  66
    TX_TILEMAP(TX_TILES_STONE_11,                1, 1, GFX_DUNGEON_TS, 152, 16, 8, 8),  //  67
    TX_PLACEHOLDER(TX_TILES_GRASS_14,                1, 1),  //  68
    TX_TILEMAP(TX_TILES_STONE_07,                1, 1, GFX_DUNGEON_TS, 112, 24, 8, 8),  //  69
    TX_PLACEHOLDER(TX_TILES_DIRT_GRASS_08,           1, 1),  //  70
    TX_PLACEHOLDER(TX_TILES_GRASS_05,                1, 1),  //  71
    TX_PLACEHOLDER(TX_TILES_GRASS_02,                1, 1),  //  72
    TX_TILEMAP(TX_TILES_STONE_00,                1, 1, GFX_DUNGEON_TS, 104, 16, 8, 8),  //  73
    TX_PLACEHOLDER(TX_TILES_DIRT_GRASS_01,           1, 1),  //  74
    TX_TILEMAP(TX_TILES_STONE_09,                1, 1, GFX_DUNGEON_TS, 128, 24, 8, 8),  //  75
    TX_PLACEHOLDER(TX_TILES_TOWN_INTERIOR_FLOOR_00,  1, 1),  //  76
    TX_PLACEHOLDER(TX_TILES_DIRT_GRASS_06,           1, 1),  //  77
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_D_01,         1, 1),  //  78
    TX_PLACEHOLDER(TX_TILES_STONE_GRASS_11,          1, 1),  //  79
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_STONE_01,      1, 1),  //  80
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_GRASS_03,      1, 1),  //  81
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_GRASS_04,      1, 1),  //  82
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_STONE_06,      1, 1),  //  83
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_UL_05,        1, 1),  //  84
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_STONE_08,      1, 1),  //  85
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_UR_03,        1, 1),  //  86
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_UR_04,        1, 1),  //  87
    TX_PLACEHOLDER(TX_TILES_WATER_00,                1, 1),  //  88
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_UL_02,        1, 1),  //  89
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_C_00,         1, 1),  //  90
    TX_PLACEHOLDER(TX_TILES_GRASS_01,                1, 1),  //  91
    TX_TILEMAP(TX_TILES_STONE_03,                1, 1, GFX_DUNGEON_TS, 128, 16, 8, 8),  //  92
    TX_TILEMAP(TX_TILES_WALL_STONE_00,           1, 1, GFX_DUNGEON_TS, 40, 40, 8, 8),  //  93
    TX_TILEMAP(TX_TILES_STONE_04,                1, 1, GFX_DUNGEON_TS, 136, 16, 8, 8),  //  94
    TX_PLACEHOLDER(TX_TILES_GRASS_06,                1, 1),  //  95
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_D_02,         1, 1),  //  96
    TX_PLACEHOLDER(TX_TILES_DIRT_GRASS_05,           1, 1),  //  97
    TX_PLACEHOLDER(TX_TILES_GRASS_08,                1, 1),  //  98
    TX_PLACEHOLDER(TX_TILES_STONE_GRASS_12,          1, 1),  //  99
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_D_05,         1, 1),  // 100
    TX_PLACEHOLDER(TX_TILES_DIRT_GRASS_02,           1, 1),  // 101
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_GRASS_07,      1, 1),  // 102
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_STONE_05,      1, 1),  // 103
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_STONE_02,      1, 1),  // 104
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_GRASS_00,      1, 1),  // 105
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_UL_01,        1, 1),  // 106
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_GRASS_09,      1, 1),  // 107
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_UR_00,        1, 1),  // 108
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_U_03,         1, 1),  // 109
    TX_PLACEHOLDER(TX_TILES_DIRT_07,                 1, 1),  // 110
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_U_04,         1, 1),  // 111
    TX_PLACEHOLDER(TX_TILES_DIRT_00,                 1, 1),  // 112
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_STONE_13,      1, 1),  // 113
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_R_01,         1, 1),  // 114
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_GRASS_11,      1, 1),  // 115
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_DR_01,        1, 1),  // 116
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_L_00,         1, 1),  // 117
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_DL_00,        1, 1),  // 118
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_STONE_14,      1, 1),  // 119
    TX_PLACEHOLDER(TX_TILES_TOWN_STONE_00,           1, 1),  // 120
    TX_PLACEHOLDER(TX_TILES_STONE_GRASS_04,          1, 1),  // 121
    TX_PLACEHOLDER(TX_TILES_DIRT_GRASS_13,           1, 1),  // 122
    TX_PLACEHOLDER(TX_TILES_GRASS_19,                1, 1),  // 123
    TX_PLACEHOLDER(TX_TILES_STONE_GRASS_03,          1, 1),  // 124
    TX_PLACEHOLDER(TX_TILES_DIRT_GRASS_14,           1, 1),  // 125
    TX_PLACEHOLDER(TX_TILES_GRASS_17,                1, 1),  // 126
    TX_PLACEHOLDER(TX_TILES_GRASS_10,                1, 1),  // 127
    TX_PLACEHOLDER(TX_TILES_STONE_12,                1, 1),  // 128
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_GRASS_08,      1, 1),  // 129
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_UR_01,        1, 1),  // 130
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_UL_00,        1, 1),  // 131
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_STONE_03,      1, 1),  // 132
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_GRASS_01,      1, 1),  // 133
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_GRASS_06,      1, 1),  // 134
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_STONE_04,      1, 1),  // 135
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_D_04,         1, 1),  // 136
    TX_PLACEHOLDER(TX_TILES_DIRT_GRASS_03,           1, 1),  // 137
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_D_03,         1, 1),  // 138
    TX_PLACEHOLDER(TX_TILES_DIRT_GRASS_04,           1, 1),  // 139
    TX_PLACEHOLDER(TX_TILES_GRASS_09,                1, 1),  // 140
    TX_PLACEHOLDER(TX_TILES_STONE_GRASS_13,          1, 1),  // 141
    TX_TILEMAP(TX_TILES_STONE_05,                1, 1, GFX_DUNGEON_TS, 144, 16, 8, 8),  // 142
    TX_PLACEHOLDER(TX_TILES_GRASS_07,                1, 1),  // 143
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_C_01,         1, 1),  // 144
    TX_PLACEHOLDER(TX_TILES_GRASS_00,                1, 1),  // 145
    TX_TILEMAP(TX_TILES_STONE_02,                1, 1, GFX_DUNGEON_TS, 120, 16, 8, 8),  // 146
    TX_TILEMAP(TX_TILES_WALL_STONE_01,           1, 1, GFX_DUNGEON_TS, 48, 40, 8, 8),  // 147
    TX_PLACEHOLDER(TX_TILES_GRASS_11,                1, 1),  // 148
    TX_PLACEHOLDER(TX_TILES_GRASS_16,                1, 1),  // 149
    TX_PLACEHOLDER(TX_TILES_GRASS_18,                1, 1),  // 150
    TX_PLACEHOLDER(TX_TILES_STONE_GRASS_02,          1, 1),  // 151
    TX_PLACEHOLDER(TX_TILES_STONE_GRASS_05,          1, 1),  // 152
    TX_PLACEHOLDER(TX_TILES_DIRT_GRASS_12,           1, 1),  // 153
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_L_01,         1, 1),  // 154
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_STONE_15,      1, 1),  // 155
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_DL_01,        1, 1),  // 156
    TX_PLACEHOLDER(TX_TILES_TOWN_STONE_01,           1, 1),  // 157
    TX_PLACEHOLDER(TX_TILES_DIRT_08,                 1, 1),  // 158
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_STONE_12,      1, 1),  // 159
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_R_00,         1, 1),  // 160
    TX_PLACEHOLDER(TX_TILES_TOWN_DIRT_GRASS_10,      1, 1),  // 161
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_DR_00,        1, 1),  // 162
    TX_PLACEHOLDER(TX_TILES_DIRT_01,                 1, 1),  // 163
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_U_05,         1, 1),  // 164
    TX_PLACEHOLDER(TX_TILES_DIRT_06,                 1, 1),  // 165
    TX_PLACEHOLDER(TX_TILES_STONE_DIRT_U_02,         1, 1),  // 166

    /* =============================================================
     *  SWITCHES  (IDs 167-177) — 32x32 placeholder
     * ============================================================= */
    TX_PLACEHOLDER(TX_SWITCHES_WALL_SWITCH_UP_00,       1, 1),  // 167
    TX_PLACEHOLDER(TX_SWITCHES_PRESSURE_PLATE_DOWN_00,  1, 1),  // 168
    TX_PLACEHOLDER(TX_SWITCHES_WALL_SWITCH_DOWN_00,     1, 1),  // 169
    TX_PLACEHOLDER(TX_SWITCHES_PRESSURE_PLATE_00,       1, 1),  // 170
    TX_PLACEHOLDER(TX_SWITCHES_PRESSURE_PLATE_DOWN_01,  1, 1),  // 171
    TX_PLACEHOLDER(TX_SWITCHES_LEVER_RIGHT_00,          1, 1),  // 172
    TX_PLACEHOLDER(TX_SWITCHES_WALL_SWITCH_CENTER_00,   1, 1),  // 173
    TX_PLACEHOLDER(TX_SWITCHES_LEVER_LEFT_00,           1, 1),  // 174
    TX_PLACEHOLDER(TX_SWITCHES_LEVER_CENTER_00,         1, 1),  // 175
    TX_PLACEHOLDER(TX_SWITCHES_PRESSURE_PLATE_UP_01,    1, 1),  // 176
    TX_PLACEHOLDER(TX_SWITCHES_PRESSURE_PLATE_UP_00,    1, 1),  // 177

    /* =============================================================
     *  GEMS  (IDs 178-181) — 128x32 animated (1ctx, 4fr)
     * ============================================================= */
    TX_PLACEHOLDER(TX_GEMS_EMERALD,  1, 4),  // 178
    TX_PLACEHOLDER(TX_GEMS_RUBY,     1, 4),  // 179
    TX_PLACEHOLDER(TX_GEMS_DIAMOND,  1, 4),  // 180
    TX_PLACEHOLDER(TX_GEMS_SAPPHIRE, 1, 4),  // 181

    /* =============================================================
     *  TILE ETCHINGS  (ID 182) — 32x32 placeholder
     * ============================================================= */
    TX_PLACEHOLDER(TX_TILE_ETCHINGS_SUMMONING_CIRCLE_SHEET, 1, 1),  // 182

    /* =============================================================
     *  SHIELDS  (IDs 183-185) — 32x32 placeholder
     * ============================================================= */
    TX_PLACEHOLDER(TX_SHIELD_BUCKLER,             1, 1),  // 183
    TX_PLACEHOLDER(TX_SHIELD_KITE_SHIELD,         1, 1),  // 184
    TX_PLACEHOLDER(TX_SHIELD_WOODEN_TOWER_SHIELD, 1, 1),  // 185

    /* =============================================================
     *  ITEMS  (IDs 186-196) — 32x32 placeholder
     * ============================================================= */
    TX_PLACEHOLDER(TX_ITEM_ARROW,            1, 1),  // 186
    TX_PLACEHOLDER(TX_ITEM_TORCH,            1, 1),  // 187
    TX_PLACEHOLDER(TX_ITEM_KEY,              1, 1),  // 188
    TX_PLACEHOLDER(TX_ITEM_SACK_00,          1, 1),  // 189
    TX_PLACEHOLDER(TX_ITEM_POTION_HP_MEDIUM, 1, 1),  // 190
    TX_PLACEHOLDER(TX_ITEM_KEY_BASIC,        1, 1),  // 191
    TX_PLACEHOLDER(TX_ITEM_POTION_MP_SMALL,  1, 1),  // 192
    TX_PLACEHOLDER(TX_ITEM_POTION_MP_LARGE,  1, 1),  // 193
    TX_PLACEHOLDER(TX_ITEM_POTION_HP_LARGE,  1, 1),  // 194
    TX_PLACEHOLDER(TX_ITEM_POTION_HP_SMALL,  1, 1),  // 195
    TX_PLACEHOLDER(TX_ITEM_POTION_MP_MEDIUM, 1, 1),  // 196

    /* =============================================================
     *  RINGS  (IDs 197-199) — 128x32 animated (1ctx, 4fr)
     * ============================================================= */
    TX_PLACEHOLDER(TX_RING_SILVER, 1, 4),  // 197
    TX_PLACEHOLDER(TX_RING_IRON,   1, 4),  // 198
    TX_PLACEHOLDER(TX_RING_GOLD,   1, 4),  // 199

    /* =============================================================
     *  GUI  (IDs 200-205) — 32x32 placeholder
     * ============================================================= */
    TX_PLACEHOLDER(TX_GUI_ICONS_HEART_00,         1, 1),  // 200
    TX_PLACEHOLDER(TX_GUI_SELECTORS_GREEN_00,     1, 1),  // 201
    TX_PLACEHOLDER(TX_GUI_SELECTORS_GREEN_01,     1, 1),  // 202
    TX_PLACEHOLDER(TX_GUI_SELECTORS_GREEN_02,     1, 1),  // 203
    TX_PLACEHOLDER(TX_GUI_SELECTORS_GREEN_03,     1, 1),  // 204
    TX_PLACEHOLDER(TX_GUI_INVENTORY_ITEM_SLOT_00, 1, 1),  // 205

    /* =============================================================
     *  CHARACTER SPRITES — HUMAN  (base + shadows)
     *
     *  Idle:           512x128  → 4ctx 16fr
     *  Walk:           128x128  → 4ctx  4fr
     *  Attack:         128x128  → 4ctx  4fr
     *  Jump:           128x128  → 4ctx  4fr
     *  Dmg:            128x128  → 4ctx  4fr
     *  SpinDie:        384x32   → 1ctx 12fr
     *  SoulDie:        384x32   → 1ctx 12fr
     *  ChargedAttack:  192x96   → 3ctx  6fr
     * ============================================================= */
    TX_FILE(TX_CHAR_HUMAN_IDLE,              4, 16, GFX_HUMAN "HumanIdle.png"),                    // 225
    TX_FILE(TX_CHAR_HUMAN_IDLE_SHADOW,       4, 16, GFX_HUMAN_SH "ShadowHumanoidIdle.png"),        // 336
    TX_FILE(TX_CHAR_HUMAN_WALK,              4,  4, GFX_HUMAN "HumanWalk.png"),                    // 348
    TX_FILE(TX_CHAR_HUMAN_WALK_SHADOW,       4,  4, GFX_HUMAN_SH "ShadowHumanoidWalk.png"),        // 357
    TX_FILE(TX_CHAR_HUMAN_JUMP,              4,  4, GFX_HUMAN "HumanJump.png"),                    // 239
    TX_FILE(TX_CHAR_HUMAN_JUMP_SHADOW,       4,  4, GFX_HUMAN_SH "ShadowHumanoidJump.png"),        // 329
    TX_FILE(TX_CHAR_HUMAN_DMG,               4,  4, GFX_HUMAN "HumanDmg.png"),                    // 295
    TX_FILE(TX_CHAR_HUMAN_DMG_SHADOW,        4,  4, GFX_HUMAN_SH "ShadowHumanoidDmg.png"),         // 320
    TX_FILE(TX_CHAR_HUMAN_SPINDIE,           1, 12, GFX_HUMAN "HumanSpinDie.png"),                 // 248
    TX_FILE(TX_CHAR_HUMAN_SPINDIE_SHADOW,    1, 12, GFX_HUMAN_SH "ShadowHumanoidSpinDie.png"),     // 308
    TX_FILE(TX_CHAR_HUMAN_SOULDIE,           1, 12, GFX_HUMAN "HumanSoulDie.png"),                 // 253
    TX_FILE(TX_CHAR_HUMAN_SOULDIE_SHADOW,    1, 12, GFX_HUMAN_SH "ShadowHumanoidSoulDie.png"),     // 341
    TX_FILE(TX_CHAR_HUMAN_CHARGEDATTACK_SHADOW,  3,  6, GFX_HUMAN_SH "ShadowHumanoidChargedAttack.png"), // 261
    /* Human ATTACK_SLASH: use base Attack spritesheet */
    TX_FILE(TX_CHAR_HUMAN_ATTACK_SLASH,          4,  4, GFX_HUMAN "HumanAttack.png"),              // 258
    TX_FILE(TX_CHAR_HUMAN_ATTACK_SLASH_SHADOW,   4,  4, GFX_HUMAN_SH "ShadowHumanoidAttack.png"),  // 313
    /* Human ATTACK_THRUST: placeholder */
    TX_PLACEHOLDER(TX_CHAR_HUMAN_ATTACK_THRUST,         4, 4),  // 252
    TX_PLACEHOLDER(TX_CHAR_HUMAN_ATTACK_THRUST_SHADOW,  4, 4),  // 289
    /* Human GUARD: placeholder */
    TX_PLACEHOLDER(TX_CHAR_HUMAN_GUARD,                 4, 4),  // 216
    TX_PLACEHOLDER(TX_CHAR_HUMAN_GUARD_SHADOW,          4, 4),  // 274
    TX_PLACEHOLDER(TX_CHAR_HUMAN_GUARD_SUCCESS,         4, 4),  // 284
    TX_PLACEHOLDER(TX_CHAR_HUMAN_GUARD_SUCCESS_SHADOW,  4, 4),  // 360
    /* Human SHOT: placeholder */
    TX_PLACEHOLDER(TX_CHAR_HUMAN_SHOT,                  4, 4),  // 262
    TX_PLACEHOLDER(TX_CHAR_HUMAN_SHOT_SHADOW,           4, 4),  // 351
    /* Human MINING: placeholder */
    TX_PLACEHOLDER(TX_CHAR_HUMAN_MINING,                4, 4),  // 343

    /* =============================================================
     *  CHARACTER SPRITES — ORC
     *
     *  Idle:           512x128  → 4ctx 16fr
     *  Walk:           128x128  → 4ctx  4fr
     *  BaseAttack:     128x128  → 4ctx  4fr
     *  Jump:           128x128  → 4ctx  4fr
     *  Dmg:            128x128  → 4ctx  4fr
     *  Die:            384x32   → 1ctx 12fr
     *  ChargedAttack:  192x96   → 3ctx  6fr
     * ============================================================= */
    TX_FILE(TX_CHAR_ORC_IDLE,                4, 16, GFX_ORC "OrcIdle.png"),                        // 291
    TX_FILE(TX_CHAR_ORC_IDLE_SHADOW,         4, 16, GFX_ORC_SH "ShadowHumanoidIdle.png"),          // 361
    TX_FILE(TX_CHAR_ORC_WALK,                4,  4, GFX_ORC "OrcWalk.png"),                        // 287
    TX_FILE(TX_CHAR_ORC_WALK_SHADOW,         4,  4, GFX_ORC_SH "ShadowHumanoidWalk.png"),          // 342
    TX_FILE(TX_CHAR_ORC_ATTACK,              4,  4, GFX_ORC "OrcBaseAttack.png"),                  // 297
    TX_FILE(TX_CHAR_ORC_ATTACK_SHADOW,       4,  4, GFX_ORC_SH "ShadowHumanoidAttack.png"),        // 317
    TX_FILE(TX_CHAR_ORC_JUMP,                4,  4, GFX_ORC "OrcJump.png"),                        // 323
    TX_FILE(TX_CHAR_ORC_JUMP_SHADOW,         4,  4, GFX_ORC_SH "ShadowHumanoidJump.png"),          // 352
    TX_FILE(TX_CHAR_ORC_DMG,                 4,  4, GFX_ORC "OrcDmg.png"),                         // 282
    TX_FILE(TX_CHAR_ORC_DMG_SHADOW,          4,  4, GFX_ORC_SH "ShadowHumanoidDmg.png"),           // 224
    TX_FILE(TX_CHAR_ORC_DIE,                 1, 12, GFX_ORC "OrcDie.png"),                         // 338
    TX_FILE(TX_CHAR_ORC_DIE_SHADOW,          1, 12, GFX_ORC_SH "ShadowHumanoidSpinDie.png"),       // 217
    TX_FILE(TX_CHAR_ORC_CHARGEDATTACK,           3,  6, GFX_ORC "OrcChargedAttack.png"),               // 333
    TX_FILE(TX_CHAR_ORC_CHARGEDATTACK_SHADOW,    3,  6, GFX_ORC_SH "ShadowHumanoidChargedAttack.png"),
    /* Orc ATTACK_SLASH: use base attack */
    TX_FILE(TX_CHAR_ORC_ATTACK_SLASH,            4,  4, GFX_ORC "OrcBaseAttack.png"),              // 229
    TX_FILE(TX_CHAR_ORC_ATTACK_SLASH_SHADOW,     4,  4, GFX_ORC_SH "ShadowHumanoidAttack.png"),    // 314
    /* Orc ATTACK_THRUST: placeholder */
    TX_PLACEHOLDER(TX_CHAR_ORC_ATTACK_THRUST,         4, 4),  // 365
    TX_PLACEHOLDER(TX_CHAR_ORC_ATTACK_THRUST_SHADOW,  4, 4),  // 214
    /* Orc GUARD: placeholder */
    TX_PLACEHOLDER(TX_CHAR_ORC_GUARD,                 4, 4),  // 263
    TX_PLACEHOLDER(TX_CHAR_ORC_GUARD_SHADOW,          4, 4),  // 250
    TX_PLACEHOLDER(TX_CHAR_ORC_GUARD_SUCCESS,         4, 4),  // 327
    TX_PLACEHOLDER(TX_CHAR_ORC_GUARD_SUCCESS_SHADOW,  4, 4),  // 281
    /* Orc SHOT: placeholder */
    TX_PLACEHOLDER(TX_CHAR_ORC_SHOT,                  4, 4),  // 334
    TX_PLACEHOLDER(TX_CHAR_ORC_SHOT_SHADOW,           4, 4),  // 330
    /* Orc MINING: placeholder */
    TX_PLACEHOLDER(TX_CHAR_ORC_MINING,                4, 4),  // 277

    /* =============================================================
     *  CHARACTER SPRITES — ELF
     *
     *  Same dimensions as Human (humanoid rig):
     *  Idle 512x128, Walk/Attack/Jump/Dmg 128x128,
     *  SpinDie 384x32, ChargedAttack 192x96
     *  (No SoulDie for Elf)
     * ============================================================= */
    TX_FILE(TX_CHAR_ELF_IDLE,                4, 16, GFX_ELF "ElfIdle.png"),                        // 228
    TX_FILE(TX_CHAR_ELF_IDLE_SHADOW,         4, 16, GFX_ELF_SH "ShadowHumanoidIdle.png"),          // 249
    TX_FILE(TX_CHAR_ELF_WALK,                4,  4, GFX_ELF "ElfWalk.png"),                        // 346
    TX_FILE(TX_CHAR_ELF_WALK_SHADOW,         4,  4, GFX_ELF_SH "ShadowHumanoidWalk.png"),          // 227
    TX_FILE(TX_CHAR_ELF_ATTACK,              4,  4, GFX_ELF "ElfAttack.png"),                      // 301
    TX_FILE(TX_CHAR_ELF_ATTACK_SHADOW,       4,  4, GFX_ELF_SH "ShadowHumanoidAttack.png"),        // 349
    TX_FILE(TX_CHAR_ELF_JUMP,                4,  4, GFX_ELF "ElfJump.png"),                        // 236
    TX_FILE(TX_CHAR_ELF_JUMP_SHADOW,         4,  4, GFX_ELF_SH "ShadowHumanoidJump.png"),          // 237
    TX_FILE(TX_CHAR_ELF_DMG,                 4,  4, GFX_ELF "ElfDmg.png"),                         // 251
    TX_FILE(TX_CHAR_ELF_DMG_SHADOW,          4,  4, GFX_ELF_SH "ShadowHumanoidDmg.png"),           // 312
    TX_FILE(TX_CHAR_ELF_SPINDIE,             1, 12, GFX_ELF "ElfSpinDie.png"),                     // 211
    TX_FILE(TX_CHAR_ELF_SPINDIE_SHADOW,      1, 12, GFX_ELF_SH "ShadowHumanoidSpinDie.png"),       // 347
    TX_FILE(TX_CHAR_ELF_CHARGEDATTACK,           3,  6, GFX_ELF "ElfChargedAttack.png"),               // 298
    TX_FILE(TX_CHAR_ELF_CHARGEDATTACK_SHADOW,    3,  6, GFX_ELF_SH "ShadowHumanoidChargedAttack.png"), // 222
    /* Elf has no SoulDie — handled by ElfSpinDie in game logic */
    TX_FILE(TX_CHAR_ELF_SOULDIE_SHADOW,      1, 12, GFX_ELF_SH "ShadowHumanoidSoulDie.png"),       // 299
    /* Elf ATTACK_SLASH: use base attack */
    TX_FILE(TX_CHAR_ELF_ATTACK_SLASH,            4,  4, GFX_ELF "ElfAttack.png"),                  // 255
    TX_FILE(TX_CHAR_ELF_ATTACK_SLASH_SHADOW,     4,  4, GFX_ELF_SH "ShadowHumanoidAttack.png"),    // 358
    /* Elf ATTACK_THRUST: placeholder */
    TX_PLACEHOLDER(TX_CHAR_ELF_ATTACK_THRUST,         4, 4),  // 307
    TX_PLACEHOLDER(TX_CHAR_ELF_ATTACK_THRUST_SHADOW,  4, 4),  // 331
    /* Elf GUARD: placeholder */
    TX_PLACEHOLDER(TX_CHAR_ELF_GUARD,                 4, 4),  // 276
    TX_PLACEHOLDER(TX_CHAR_ELF_GUARD_SHADOW,          4, 4),  // 271
    TX_PLACEHOLDER(TX_CHAR_ELF_GUARD_SUCCESS,         4, 4),  // 302
    TX_PLACEHOLDER(TX_CHAR_ELF_GUARD_SUCCESS_SHADOW,  4, 4),  // 315
    /* Elf SHOT: placeholder */
    TX_PLACEHOLDER(TX_CHAR_ELF_SHOT,                  4, 4),  // 264
    TX_PLACEHOLDER(TX_CHAR_ELF_SHOT_SHADOW,           4, 4),  // 206
    /* Elf MINING: placeholder */
    TX_PLACEHOLDER(TX_CHAR_ELF_MINING,                4, 4),  // 286

    /* =============================================================
     *  CHARACTER SPRITES — DWARF (Base Dwarf)
     *
     *  Same humanoid rig except:
     *  SpinDie 352x32 → 1ctx 11fr
     *  Shadow filenames have "Dwar" typo (ShadowDwarJump, etc.)
     * ============================================================= */
    TX_FILE(TX_CHAR_DWARF_IDLE,              4, 16, GFX_DWARF "DwarfIdle.png"),                    // 353
    TX_FILE(TX_CHAR_DWARF_IDLE_SHADOW,       4, 16, GFX_DWARF_SH "ShadowDwarfIdle.png"),           // 221
    TX_FILE(TX_CHAR_DWARF_WALK,              4,  4, GFX_DWARF "DwarfWalk.png"),                    // 219
    TX_FILE(TX_CHAR_DWARF_WALK_SHADOW,       4,  4, GFX_DWARF_SH "ShadowDwarWalk.png"),            // 245
    TX_FILE(TX_CHAR_DWARF_ATTACK,            4,  4, GFX_DWARF "DwarfAttack.png"),                  // 362
    TX_FILE(TX_CHAR_DWARF_ATTACK_SHADOW,     4,  4, GFX_DWARF_SH "ShadowDwarfAttack.png"),         // 306
    TX_FILE(TX_CHAR_DWARF_JUMP,              4,  4, GFX_DWARF "DwarfJump.png"),                    // 339
    TX_FILE(TX_CHAR_DWARF_JUMP_SHADOW,       4,  4, GFX_DWARF_SH "ShadowDwarJump.png"),            // 209
    TX_FILE(TX_CHAR_DWARF_DMG,               4,  4, GFX_DWARF "DwarfDmg.png"),                    // 363
    TX_FILE(TX_CHAR_DWARF_DMG_SHADOW,        4,  4, GFX_DWARF_SH "ShadowDwarfDmg.png"),            // 212
    TX_FILE(TX_CHAR_DWARF_SPINDIE,           1, 11, GFX_DWARF "DwarfSpinDie.png"),                 // 359
    TX_FILE(TX_CHAR_DWARF_SPINDIE_SHADOW,    1, 11, GFX_DWARF_SH "ShadowDwarSpinDie.png"),         // 354
    TX_FILE(TX_CHAR_DWARF_CHARGEDATTACK,         3,  6, GFX_DWARF "DwarfChargedAttack.png"),           // 345
    TX_FILE(TX_CHAR_DWARF_CHARGEDATTACK_SHADOW,  3,  6, GFX_DWARF_SH "ShadowDwarfChargedAttack.png"), // 366
    /* Dwarf ATTACK_SLASH: use base attack */
    TX_FILE(TX_CHAR_DWARF_ATTACK_SLASH,          4,  4, GFX_DWARF "DwarfAttack.png"),              // 300
    TX_FILE(TX_CHAR_DWARF_ATTACK_SLASH_SHADOW,   4,  4, GFX_DWARF_SH "ShadowDwarfAttack.png"),     // 273
    /* Dwarf ATTACK_THRUST: placeholder */
    TX_PLACEHOLDER(TX_CHAR_DWARF_ATTACK_THRUST,         4, 4),  // 350
    TX_PLACEHOLDER(TX_CHAR_DWARF_ATTACK_THRUST_SHADOW,  4, 4),  // 241
    /* Dwarf GUARD: placeholder */
    TX_PLACEHOLDER(TX_CHAR_DWARF_GUARD,                 4, 4),  // 265
    TX_PLACEHOLDER(TX_CHAR_DWARF_GUARD_SHADOW,          4, 4),  // 325
    TX_PLACEHOLDER(TX_CHAR_DWARF_GUARD_SUCCESS,         4, 4),  // 337
    TX_PLACEHOLDER(TX_CHAR_DWARF_GUARD_SUCCESS_SHADOW,  4, 4),  // 266
    /* Dwarf SHOT: placeholder */
    TX_PLACEHOLDER(TX_CHAR_DWARF_SHOT,                  4, 4),  // 309
    TX_PLACEHOLDER(TX_CHAR_DWARF_SHOT_SHADOW,           4, 4),  // 240
    /* Dwarf MINING: placeholder */
    TX_PLACEHOLDER(TX_CHAR_DWARF_MINING,                4, 4),  // 226

    /* =============================================================
     *  CHARACTER SPRITES — DWARF2 (Yellow Beard)
     *
     *  Note: "YellowBeardIdle" (with 'd') but "YellowBear*" (no 'd')
     *  for all other animations.  Shadows reuse Base_Dwarf shadows.
     *  SpinDie 384x32 → 1ctx 12fr
     * ============================================================= */
    TX_FILE(TX_CHAR_DWARF2_IDLE,             4, 16, GFX_DWARF2 "YellowBeardIdle.png"),             // 321
    TX_FILE(TX_CHAR_DWARF2_WALK,             4,  4, GFX_DWARF2 "YellowBearWalk.png"),              // 257
    TX_FILE(TX_CHAR_DWARF2_ATTACK,           4,  4, GFX_DWARF2 "YellowBearAttack.png"),            // 244
    TX_FILE(TX_CHAR_DWARF2_JUMP,             4,  4, GFX_DWARF2 "YellowBearJump.png"),              // 292
    TX_FILE(TX_CHAR_DWARF2_DMG,              4,  4, GFX_DWARF2 "YellowBearDmg.png"),               // 280
    TX_FILE(TX_CHAR_DWARF2_SPINDIE,          1, 12, GFX_DWARF2 "YellowBearSpinDie.png"),           // 340
    TX_FILE(TX_CHAR_DWARF2_SPINDIE2,         1, 12, GFX_DWARF2 "YellowBearSpinDie.png"),           // 316 (alias)
    TX_FILE(TX_CHAR_DWARF2_CHARGEDATTACK,    3,  6, GFX_DWARF2 "YellowBearChargedAttack.png"),     // 223

    /* =============================================================
     *  CHARACTER SPRITES — HALFLING
     *
     *  Same humanoid rig: Idle 512x128, Walk/Attack/Jump/Dmg 128x128,
     *  SpinDie 384x32 → 1ctx 12fr, ChargedAttack 192x96 → 3ctx 6fr
     * ============================================================= */
    TX_FILE(TX_CHAR_HALFLING_IDLE,               4, 16, GFX_HALFLING "HalflingIdle.png"),                   // 318
    TX_FILE(TX_CHAR_HALFLING_IDLE_SHADOW,        4, 16, GFX_HALFLING_SH "ShadowHumanoidIdle.png"),          // 246
    TX_FILE(TX_CHAR_HALFLING_WALK,               4,  4, GFX_HALFLING "HalflingWalk.png"),                   // 256
    TX_FILE(TX_CHAR_HALFLING_WALK_SHADOW,        4,  4, GFX_HALFLING_SH "ShadowHumanoidWalk.png"),          // 220
    TX_FILE(TX_CHAR_HALFLING_ATTACK,             4,  4, GFX_HALFLING "HalflingAttack.png"),                 // 285
    TX_FILE(TX_CHAR_HALFLING_ATTACK_SHADOW,      4,  4, GFX_HALFLING_SH "ShadowHumanoidAttack.png"),        // 335
    TX_FILE(TX_CHAR_HALFLING_JUMP,               4,  4, GFX_HALFLING "HalflingJump.png"),                   // 290
    TX_FILE(TX_CHAR_HALFLING_JUMP_SHADOW,        4,  4, GFX_HALFLING_SH "ShadowHumanoidJump.png"),          // 233
    TX_FILE(TX_CHAR_HALFLING_DMG,                4,  4, GFX_HALFLING "HalflingDmg.png"),                    // 272
    TX_FILE(TX_CHAR_HALFLING_DMG_SHADOW,         4,  4, GFX_HALFLING_SH "ShadowHumanoidDmg.png"),           // 279
    TX_FILE(TX_CHAR_HALFLING_SPINDIE,            1, 12, GFX_HALFLING "HalflingSpinDie.png"),                // 230
    TX_FILE(TX_CHAR_HALFLING_SPINDIE_SHADOW,     1, 12, GFX_HALFLING_SH "ShadowHumanoidSpinDie.png"),       // 235
    TX_FILE(TX_CHAR_HALFLING_SOULDIE_SHADOW,     1, 12, GFX_HALFLING_SH "ShadowHumanoidSoulDie.png"),       // 268
    TX_FILE(TX_CHAR_HALFLING_CHARGEDATTACK,          3,  6, GFX_HALFLING "HalflingChargedAttack.png"),           // 322
    TX_FILE(TX_CHAR_HALFLING_CHARGEDATTACK_SHADOW,   3,  6, GFX_HALFLING_SH "ShadowHumanoidChargedAttack.png"), // 305
    /* Halfling ATTACK_SLASH: use base attack */
    TX_FILE(TX_CHAR_HALFLING_ATTACK_SLASH,           4,  4, GFX_HALFLING "HalflingAttack.png"),             // 296
    TX_FILE(TX_CHAR_HALFLING_ATTACK_SLASH_SHADOW,    4,  4, GFX_HALFLING_SH "ShadowHumanoidAttack.png"),    // 234
    /* Halfling ATTACK_THRUST: placeholder */
    TX_PLACEHOLDER(TX_CHAR_HALFLING_ATTACK_THRUST,         4, 4),  // 288
    TX_PLACEHOLDER(TX_CHAR_HALFLING_ATTACK_THRUST_SHADOW,  4, 4),  // 275
    /* Halfling GUARD: placeholder */
    TX_PLACEHOLDER(TX_CHAR_HALFLING_GUARD,                 4, 4),  // 304
    TX_PLACEHOLDER(TX_CHAR_HALFLING_GUARD_SHADOW,          4, 4),  // 310
    TX_PLACEHOLDER(TX_CHAR_HALFLING_GUARD_SUCCESS,         4, 4),  // 326
    TX_PLACEHOLDER(TX_CHAR_HALFLING_GUARD_SUCCESS_SHADOW,  4, 4),  // 208
    /* Halfling SHOT: placeholder */
    TX_PLACEHOLDER(TX_CHAR_HALFLING_SHOT,                  4, 4),  // 364
    TX_PLACEHOLDER(TX_CHAR_HALFLING_SHOT_SHADOW,           4, 4),  // 210
    /* Halfling MINING: placeholder */
    TX_PLACEHOLDER(TX_CHAR_HALFLING_MINING,                4, 4),  // 303

    /* =============================================================
     *  CHARACTER SPRITES — GOBLIN
     *
     *  Same humanoid rig as Halfling.
     *  SpinDie 384x32 → 1ctx 12fr
     * ============================================================= */
    TX_FILE(TX_CHAR_GOBLIN_IDLE,                 4, 16, GFX_GOBLIN "GoblinIdle.png"),                       // 355
    TX_FILE(TX_CHAR_GOBLIN_IDLE_SHADOW,          4, 16, GFX_GOBLIN_SH "ShadowHumanoidIdle.png"),            // 260
    TX_FILE(TX_CHAR_GOBLIN_WALK,                 4,  4, GFX_GOBLIN "GoblinWalk.png"),                       // 218
    TX_FILE(TX_CHAR_GOBLIN_WALK_SHADOW,          4,  4, GFX_GOBLIN_SH "ShadowHumanoidWalk.png"),            // 270
    TX_FILE(TX_CHAR_GOBLIN_ATTACK,               4,  4, GFX_GOBLIN "GoblinAttack.png"),                    // 328
    TX_FILE(TX_CHAR_GOBLIN_ATTACK_SHADOW,        4,  4, GFX_GOBLIN_SH "ShadowHumanoidAttack.png"),          // 254
    TX_FILE(TX_CHAR_GOBLIN_JUMP,                 4,  4, GFX_GOBLIN "GoblinJump.png"),                      // 332
    TX_FILE(TX_CHAR_GOBLIN_JUMP_SHADOW,          4,  4, GFX_GOBLIN_SH "ShadowHumanoidJump.png"),            // 267
    TX_FILE(TX_CHAR_GOBLIN_DMG,                  4,  4, GFX_GOBLIN "GoblinDmg.png"),                       // 269
    TX_FILE(TX_CHAR_GOBLIN_DMG_SHADOW,           4,  4, GFX_GOBLIN_SH "ShadowHumanoidDmg.png"),             // 259
    TX_FILE(TX_CHAR_GOBLIN_SPINDIE,              1, 12, GFX_GOBLIN "GoblinSpinDie.png"),                    // 294
    TX_FILE(TX_CHAR_GOBLIN_SPINDIE_SHADOW,       1, 12, GFX_GOBLIN_SH "ShadowHumanoidSpinDie.png"),         // 283
    TX_FILE(TX_CHAR_GOBLIN_SOULDIE_SHADOW,       1, 12, GFX_GOBLIN_SH "ShadowHumanoidSoulDie.png"),         // 207
    TX_FILE(TX_CHAR_GOBLIN_CHARGEDATTACK,            3,  6, GFX_GOBLIN "GoblinChargedAttack.png"),              // 247
    TX_FILE(TX_CHAR_GOBLIN_CHARGEDATTACK_SHADOW,     3,  6, GFX_GOBLIN_SH "ShadowHumanoidChargedAttack.png"),   // 231
    /* Goblin ATTACK_SLASH: use base attack */
    TX_FILE(TX_CHAR_GOBLIN_ATTACK_SLASH,             4,  4, GFX_GOBLIN "GoblinAttack.png"),                 // 238
    TX_FILE(TX_CHAR_GOBLIN_ATTACK_SLASH_SHADOW,      4,  4, GFX_GOBLIN_SH "ShadowHumanoidAttack.png"),      // 242
    /* Goblin ATTACK_THRUST: placeholder */
    TX_PLACEHOLDER(TX_CHAR_GOBLIN_ATTACK_THRUST,         4, 4),  // 215
    TX_PLACEHOLDER(TX_CHAR_GOBLIN_ATTACK_THRUST_SHADOW,  4, 4),  // 356
    /* Goblin GUARD: placeholder */
    TX_PLACEHOLDER(TX_CHAR_GOBLIN_GUARD,                 4, 4),  // 319
    TX_PLACEHOLDER(TX_CHAR_GOBLIN_GUARD_SHADOW,          4, 4),  // 213
    TX_PLACEHOLDER(TX_CHAR_GOBLIN_GUARD_SUCCESS,         4, 4),  // 243
    TX_PLACEHOLDER(TX_CHAR_GOBLIN_GUARD_SUCCESS_SHADOW,  4, 4),  // 293
    /* Goblin SHOT: placeholder */
    TX_PLACEHOLDER(TX_CHAR_GOBLIN_SHOT,                  4, 4),  // 324
    TX_PLACEHOLDER(TX_CHAR_GOBLIN_SHOT_SHADOW,           4, 4),  // 278
    /* Goblin MINING: placeholder */
    TX_PLACEHOLDER(TX_CHAR_GOBLIN_MINING,                4, 4),  // 232

    /* =============================================================
     *  MINING SHADOW (generic) — placeholder
     * ============================================================= */
    TX_PLACEHOLDER(TX_CHAR_MINING_SHADOW,  4, 4),  // 311

    /* =============================================================
     *  CHESTS  (IDs 367-368) — 32x32 placeholder
     * ============================================================= */
    TX_PLACEHOLDER(TX_CHEST_OPEN_00,   1, 1),  // 367
    TX_PLACEHOLDER(TX_CHEST_CLOSED_00, 1, 1),  // 368

    /* =============================================================
     *  WEAPONS  (IDs 369-378) — 32x32 placeholder
     * ============================================================= */
    TX_PLACEHOLDER(TX_WEAPON_SHORT_SWORD,      1, 1),  // 369
    TX_PLACEHOLDER(TX_WEAPON_BOW,              1, 1),  // 370
    TX_PLACEHOLDER(TX_WEAPON_LONG_SWORD,       1, 1),  // 371
    TX_PLACEHOLDER(TX_WEAPON_WARHAMMER,        1, 1),  // 372
    TX_PLACEHOLDER(TX_WEAPON_ARROW,            1, 1),  // 373
    TX_PLACEHOLDER(TX_WEAPON_TWO_HANDED_SWORD, 1, 1),  // 374
    TX_PLACEHOLDER(TX_WEAPON_AXE,              1, 1),  // 375
    TX_PLACEHOLDER(TX_WEAPON_FLAIL,            1, 1),  // 376
    TX_PLACEHOLDER(TX_WEAPON_DAGGER,           1, 1),  // 377
    TX_PLACEHOLDER(TX_WEAPON_PICK_AXE,         1, 1),  // 378

    /* =============================================================
     *  PROPS  (IDs 379-395) — 32x32 placeholder
     * ============================================================= */
    TX_PLACEHOLDER(TX_PROP_WOODEN_TABLE_00,              1, 1),  // 379
    TX_PLACEHOLDER(TX_PROP_WOODEN_SIGN,                  1, 1),  // 380
    TX_PLACEHOLDER(TX_PROP_WOODEN_CHAIR_00,              1, 1),  // 381
    TX_PLACEHOLDER(TX_PROP_BANNER_01,                    1, 1),  // 382
    TX_PLACEHOLDER(TX_PROP_WOODEN_BARREL_OPEN_TOP_WATER, 1, 1),  // 383
    TX_PLACEHOLDER(TX_PROP_TORCH_00,                     1, 1),  // 384
    TX_PLACEHOLDER(TX_PROP_STATUE_00,                    1, 1),  // 385
    TX_PLACEHOLDER(TX_PROP_WOODEN_TABLE_01,              1, 1),  // 386
    TX_PLACEHOLDER(TX_PROP_WOODEN_BARREL_OPEN_TOP_EMPTY, 1, 1),  // 387
    TX_PLACEHOLDER(TX_PROP_CANDLE_00,                    1, 1),  // 388
    TX_PLACEHOLDER(TX_PROP_BANNER_00,                    1, 1),  // 389
    TX_PLACEHOLDER(TX_PROP_WHITE_STATUE_DOWN_00,         1, 1),  // 390
    TX_PLACEHOLDER(TX_PROP_JAR_00,                       1, 1),  // 391
    TX_PLACEHOLDER(TX_PROP_WOODEN_BOX_OPEN_TOP_EMPTY,    1, 1),  // 392
    TX_PLACEHOLDER(TX_PROP_WOODEN_BOX_OPEN_TOP_WATERY,   1, 1),  // 393
    TX_PLACEHOLDER(TX_PROP_BANNER_02,                    1, 1),  // 394
    TX_PLACEHOLDER(TX_PROP_PLATE_00,                     1, 1),  // 395

    /* =============================================================
     *  MONSTERS — BAT
     *
     *  FlyIdle:  64x128  → 4ctx  2fr
     *  Attack:   128x128 → 4ctx  4fr
     *  Dmg:      128x128 → 4ctx  4fr
     *  Sleep:    256x96  → 3ctx  8fr
     *  Die:      288x32  → 1ctx  9fr
     * ============================================================= */
    TX_FILE(TX_MONSTER_BAT_IDLE,           4,  2, GFX_BAT "BatFlyIdle.png"),           // 396
    TX_FILE(TX_MONSTER_BAT_IDLE_SHADOW,    4,  2, GFX_BAT_SH "ShadowBatFly.png"),      // 416
    TX_FILE(TX_MONSTER_BAT_ATTACK,         4,  4, GFX_BAT "BatAttack.png"),            // 427
    TX_FILE(TX_MONSTER_BAT_ATTACK_SHADOW,  4,  4, GFX_BAT_SH "ShadowBatAttack.png"),   // 454
    TX_FILE(TX_MONSTER_BAT_DMG,            4,  4, GFX_BAT "BatDmg.png"),               // 449
    TX_FILE(TX_MONSTER_BAT_DMG_SHADOW,     4,  4, GFX_BAT_SH "ShadowBatDmg.png"),      // 413
    TX_FILE(TX_MONSTER_BAT_SLEEP,          3,  8, GFX_BAT "BatSleep.png"),             // 436
    TX_FILE(TX_MONSTER_BAT_SLEEP_SHADOW,   3,  8, GFX_BAT_SH "ShadowBatSleep.png"),    // 432
    TX_FILE(TX_MONSTER_BAT_DIE,            1,  9, GFX_BAT "BatDie.png"),               // 402
    TX_FILE(TX_MONSTER_BAT_DIE_SHADOW,     1,  9, GFX_BAT_SH "ShadowBatDie.png"),      // 410

    /* =============================================================
     *  MONSTERS — GREEN SLIME
     *
     *  Note: actual filenames are "SlimeGreen*" not "GreenSlime*"
     *  Idle:       256x128 → 4ctx  8fr
     *  JumpAttack: 128x128 → 4ctx  4fr
     *  Dmg:        128x128 → 4ctx  4fr
     *  Die:        288x32  → 1ctx  9fr
     * ============================================================= */
    TX_FILE(TX_MONSTER_GREEN_SLIME_IDLE,                4,  8, GFX_GSLIME "SlimeGreenIdle.png"),               // 397
    TX_FILE(TX_MONSTER_GREEN_SLIME_IDLE_SHADOW,         4,  8, GFX_GSLIME_SH "ShadowSlimeIdle.png"),           // 426
    TX_FILE(TX_MONSTER_GREEN_SLIME_JUMP_ATTACK,         4,  4, GFX_GSLIME "SlimeGreenJumpAttack.png"),         // 412
    TX_FILE(TX_MONSTER_GREEN_SLIME_JUMP_ATTACK_SHADOW,  4,  4, GFX_GSLIME_SH "ShadowSlimeJump.png"),           // 421
    TX_FILE(TX_MONSTER_GREEN_SLIME_DMG,                 4,  4, GFX_GSLIME "SlimeGreenDmg.png"),                // 400
    TX_FILE(TX_MONSTER_GREEN_SLIME_DMG_SHADOW,          4,  4, GFX_GSLIME_SH "ShadowSlimeDmg.png"),            // 420
    TX_FILE(TX_MONSTER_GREEN_SLIME_DIE,                 1,  9, GFX_GSLIME "SlimeGreenDie.png"),                // 441
    TX_FILE(TX_MONSTER_GREEN_SLIME_DIE_SHADOW,          1,  9, GFX_GSLIME_SH "ShadowSlimeDie.png"),            // 423

    /* =============================================================
     *  MONSTERS — WOLF
     *
     *  Idle:  384x128 → 4ctx 12fr
     *  Walk:  128x128 → 4ctx  4fr
     *  Attack:128x128 → 4ctx  4fr
     *  Jump:  128x128 → 4ctx  4fr
     *  Dmg:   128x128 → 4ctx  4fr
     *  Die:   288x32  → 1ctx  9fr
     * ============================================================= */
    TX_FILE(TX_MONSTER_WOLF_IDLE,            4, 12, GFX_WOLF "WolfIdle.png"),              // 403
    TX_FILE(TX_MONSTER_WOLF_IDLE_SHADOW,     4, 12, GFX_WOLF_SH "ShadowWolfIdle.png"),     // 425
    TX_FILE(TX_MONSTER_WOLF_WALK,            4,  4, GFX_WOLF "WolfWalk.png"),              // 462
    TX_FILE(TX_MONSTER_WOLF_WALK_SHADOW,     4,  4, GFX_WOLF_SH "ShadowWolfWalk.png"),     // 419
    TX_FILE(TX_MONSTER_WOLF_ATTACK,          4,  4, GFX_WOLF "WolfAttack.png"),            // 443
    TX_FILE(TX_MONSTER_WOLF_ATTACK_SHADOW,   4,  4, GFX_WOLF_SH "ShadowWolfAttack.png"),   // 429
    TX_FILE(TX_MONSTER_WOLF_JUMP,            4,  4, GFX_WOLF "WolfJump.png"),              // 407
    TX_FILE(TX_MONSTER_WOLF_JUMP_SHADOW,     4,  4, GFX_WOLF_SH "ShadowWolfJump.png"),     // 431
    TX_FILE(TX_MONSTER_WOLF_DMG,             4,  4, GFX_WOLF "WolfDmg.png"),               // 415
    TX_FILE(TX_MONSTER_WOLF_DMG_SHADOW,      4,  4, GFX_WOLF_SH "ShadowWolfDmg.png"),      // 457
    TX_FILE(TX_MONSTER_WOLF_DIE,             1,  9, GFX_WOLF "WolfDie.png"),               // 439
    TX_FILE(TX_MONSTER_WOLF_DIE_SHADOW,      1,  9, GFX_WOLF_SH "ShadowWolfDie.png"),      // 453

    /* =============================================================
     *  MONSTERS — WARG
     *
     *  Idle:  384x128 → 4ctx 12fr
     *  Walk:  128x128 → 4ctx  4fr
     *  Attack:128x128 → 4ctx  4fr
     *  Jump:  128x128 → 4ctx  4fr
     *  Dmg:   128x128 → 4ctx  4fr
     *  Die:   320x32  → 1ctx 10fr
     * ============================================================= */
    TX_FILE(TX_MONSTER_WARG_IDLE,            4, 12, GFX_WARG "WargIdle.png"),              // 452
    TX_FILE(TX_MONSTER_WARG_IDLE_SHADOW,     4, 12, GFX_WARG_SH "ShadowWargIdle.png"),     // 450
    TX_FILE(TX_MONSTER_WARG_WALK,            4,  4, GFX_WARG "WargWalk.png"),              // 414
    TX_FILE(TX_MONSTER_WARG_WALK_SHADOW,     4,  4, GFX_WARG_SH "ShadowWargWalk.png"),     // 440
    TX_FILE(TX_MONSTER_WARG_ATTACK,          4,  4, GFX_WARG "WargAttack.png"),            // 398
    TX_FILE(TX_MONSTER_WARG_ATTACK_SHADOW,   4,  4, GFX_WARG_SH "ShadowWargAttack.png"),   // 456
    TX_FILE(TX_MONSTER_WARG_JUMP,            4,  4, GFX_WARG "WargJump.png"),              // 465
    TX_FILE(TX_MONSTER_WARG_JUMP_SHADOW,     4,  4, GFX_WARG_SH "ShadowWargJump.png"),     // 446
    TX_FILE(TX_MONSTER_WARG_DMG,             4,  4, GFX_WARG "WargDmg.png"),               // 406
    TX_FILE(TX_MONSTER_WARG_DMG_SHADOW,      4,  4, GFX_WARG_SH "ShadowWargDmg.png"),      // 438
    TX_FILE(TX_MONSTER_WARG_DIE,             1, 10, GFX_WARG "WargDie.png"),               // 435
    TX_FILE(TX_MONSTER_WARG_DIE_SHADOW,      1, 10, GFX_WARG_SH "ShadowWargDie.png"),      // 433

    /* =============================================================
     *  MONSTERS — SKELETON
     *
     *  Idle:   160x96  → 3ctx  5fr
     *  Walk:    64x128 → 4ctx  2fr
     *  Attack: 160x128 → 4ctx  5fr
     *  Dmg:    128x128 → 4ctx  4fr
     *  Die:    288x32  → 1ctx  9fr
     * ============================================================= */
    TX_FILE(TX_MONSTER_SKELETON_IDLE,            3,  5, GFX_SKELETON "SkeletonIdle.png"),               // 459
    TX_FILE(TX_MONSTER_SKELETON_IDLE_SHADOW,     3,  5, GFX_SKELETON_SH "ShadowSkeletonIdle.png"),      // 399
    TX_FILE(TX_MONSTER_SKELETON_WALK,            4,  2, GFX_SKELETON "SkeletonWalk.png"),               // 404
    TX_FILE(TX_MONSTER_SKELETON_WALK_SHADOW,     4,  2, GFX_SKELETON_SH "ShadowSkeletonWalk.png"),      // 408
    TX_FILE(TX_MONSTER_SKELETON_ATTACK,          4,  5, GFX_SKELETON "SkeletonAttack.png"),             // 467
    TX_FILE(TX_MONSTER_SKELETON_ATTACK_SHADOW,   4,  5, GFX_SKELETON_SH "ShadowSkeletonAttack.png"),    // 447
    TX_FILE(TX_MONSTER_SKELETON_DMG,             4,  4, GFX_SKELETON "SkeletonDmg.png"),                // 455
    TX_FILE(TX_MONSTER_SKELETON_DMG_SHADOW,      4,  4, GFX_SKELETON_SH "ShadowSkeletonDmg.png"),       // 461
    TX_FILE(TX_MONSTER_SKELETON_DIE,             1,  9, GFX_SKELETON "SkeletonDie.png"),                // 430
    TX_FILE(TX_MONSTER_SKELETON_DIE_SHADOW,      1,  9, GFX_SKELETON_SH "ShadowSkeletonDie.png"),       // 464

    /* =============================================================
     *  MONSTERS — ZOMBIE
     *
     *  Idle:   608x32  → 1ctx 19fr
     *  Walk:   128x128 → 4ctx  4fr (assumed, matching other humanoids)
     *  Attack: 160x128 → 4ctx  5fr
     *  Dmg:    128x128 → 4ctx  4fr (assumed)
     *  Die:    352x32  → 1ctx 11fr
     *
     *  Note: Zombie shadow filenames use suffix pattern
     *        "Zombie*Shadow.png" not "ShadowZombie*.png"
     * ============================================================= */
    TX_FILE(TX_MONSTER_ZOMBIE_IDLE,            1, 19, GFX_ZOMBIE "ZombieIdle.png"),                   // 422
    TX_FILE(TX_MONSTER_ZOMBIE_IDLE_SHADOW,     1, 19, GFX_ZOMBIE_SH "ZombieIdleShadow.png"),          // 434
    TX_FILE(TX_MONSTER_ZOMBIE_WALK,            4,  4, GFX_ZOMBIE "ZombieWalk.png"),                   // 444
    TX_FILE(TX_MONSTER_ZOMBIE_WALK_SHADOW,     4,  4, GFX_ZOMBIE_SH "ZombieWalkShadow.png"),          // 445
    TX_FILE(TX_MONSTER_ZOMBIE_ATTACK,          4,  5, GFX_ZOMBIE "ZombieAttack.png"),                 // 424
    TX_FILE(TX_MONSTER_ZOMBIE_ATTACK_SHADOW,   4,  5, GFX_ZOMBIE_SH "ZombieAttackShadow.png"),        // 411
    TX_FILE(TX_MONSTER_ZOMBIE_DMG,             4,  4, GFX_ZOMBIE "ZombieDmg.png"),                    // 460
    TX_FILE(TX_MONSTER_ZOMBIE_DMG_SHADOW,      4,  4, GFX_ZOMBIE_SH "ZombieDmgShadow.png"),           // 451
    TX_FILE(TX_MONSTER_ZOMBIE_DIE,             1, 11, GFX_ZOMBIE "ZombieDie.png"),                    // 417
    TX_FILE(TX_MONSTER_ZOMBIE_DIE_SHADOW,      1, 11, GFX_ZOMBIE_SH "ZombieDieShadow.png"),           // 442

    /* =============================================================
     *  MONSTERS — RAT  (no source files — all placeholders)
     *
     *  Using standard monster dimensions: Idle/Walk/Attack/Dmg 4ctx 4fr,
     *  Die 1ctx 9fr
     * ============================================================= */
    TX_PLACEHOLDER(TX_MONSTER_RAT_IDLE,            4, 4),  // 458
    TX_PLACEHOLDER(TX_MONSTER_RAT_IDLE_SHADOW,     4, 4),  // 428
    TX_PLACEHOLDER(TX_MONSTER_RAT_WALK,            4, 4),  // 405
    TX_PLACEHOLDER(TX_MONSTER_RAT_WALK_SHADOW,     4, 4),  // 418
    TX_PLACEHOLDER(TX_MONSTER_RAT_ATTACK,          4, 4),  // 448
    TX_PLACEHOLDER(TX_MONSTER_RAT_ATTACK_SHADOW,   4, 4),  // 401
    TX_PLACEHOLDER(TX_MONSTER_RAT_DMG,             4, 4),  // 409
    TX_PLACEHOLDER(TX_MONSTER_RAT_DMG_SHADOW,      4, 4),  // 466
    TX_PLACEHOLDER(TX_MONSTER_RAT_DIE,             1, 9),  // 437
    TX_PLACEHOLDER(TX_MONSTER_RAT_DIE_SHADOW,      1, 9),  // 463

    /* =============================================================
     *  AMULETS  (ID 468) — 32x32 placeholder
     * ============================================================= */
    TX_PLACEHOLDER(TX_AMULETS_AMULET_IRON, 1, 1),  // 468

    /* =============================================================
     *  ARMOR  (IDs 469-471) — 32x32 placeholder
     * ============================================================= */
    TX_PLACEHOLDER(TX_ARMOR_FEET_SHOES_LEATHER, 1, 1),  // 469
    TX_PLACEHOLDER(TX_ARMOR_BODY_SHIRT_CLOTH,   1, 1),  // 470
    TX_PLACEHOLDER(TX_ARMOR_BODY_PLATE_IRON,    1, 1),  // 471

    /* =============================================================
     *  FOOD  (IDs 472-474) — 32x32 placeholder
     * ============================================================= */
    TX_PLACEHOLDER(TX_FOOD_ONIGIRI, 1, 1),  // 472
    TX_PLACEHOLDER(TX_FOOD_BURGER,  1, 1),  // 473
    TX_PLACEHOLDER(TX_FOOD_COFFEE,  1, 1),  // 474

    /* =============================================================
     *  BOX  (ID 475) — 32x32 placeholder
     * ============================================================= */
    TX_PLACEHOLDER(TX_BOX_WOODEN, 1, 1),  // 475

    /* =============================================================
     *  RUNES  (IDs 476-483) — 32x32 placeholder
     * ============================================================= */
    TX_PLACEHOLDER(TX_RUNES_WUNJO,    1, 1),  // 476
    TX_PLACEHOLDER(TX_RUNES_KENAZ,    1, 1),  // 477
    TX_PLACEHOLDER(TX_RUNES_ANSUZ,    1, 1),  // 478
    TX_PLACEHOLDER(TX_RUNES_THURISAZ, 1, 1),  // 479
    TX_PLACEHOLDER(TX_RUNES_FEHU,     1, 1),  // 480
    TX_PLACEHOLDER(TX_RUNES_RAIDHO,   1, 1),  // 481
    TX_PLACEHOLDER(TX_RUNES_GEBO,     1, 1),  // 482
    TX_PLACEHOLDER(TX_RUNES_URUZ,     1, 1),  // 483

    /* =============================================================
     *  ACTIONS  (IDs 484-500) — 128x128 placeholder (4ctx 4fr)
     * ============================================================= */
    TX_PLACEHOLDER(TX_ACTIONS_SLASH_AXE_B,                          4, 4),  // 484
    TX_PLACEHOLDER(TX_ACTIONS_SHOT_BOW_F,                           4, 4),  // 485
    TX_PLACEHOLDER(TX_ACTIONS_GUARD_WOODEN_SHIELD_BACK,             4, 4),  // 486
    TX_PLACEHOLDER(TX_ACTIONS_GUARD_BUCKLER_SUCCESS_BACK,           4, 4),  // 487
    TX_PLACEHOLDER(TX_ACTIONS_SLASH_DAGGER_F,                       4, 4),  // 488
    TX_PLACEHOLDER(TX_ACTIONS_GUARD_TOWER_SHIELD_SUCCESS_BACK,      4, 4),  // 489
    TX_PLACEHOLDER(TX_ACTIONS_SLASH_SHORT_SWORD_B,                  4, 4),  // 490
    TX_PLACEHOLDER(TX_ACTIONS_GUARD_BUCKLER_SUCCESS_FRONT,          4, 4),  // 491
    TX_PLACEHOLDER(TX_ACTIONS_GUARD_TOWER_SHIELD_SUCCESS_FRONT,     4, 4),  // 492
    TX_PLACEHOLDER(TX_ACTIONS_SLASH_SHORT_SWORD_F,                  4, 4),  // 493
    TX_PLACEHOLDER(TX_ACTIONS_SHOT_BOW_B_HALFLING_GOBLIN_DWARF,     4, 4),  // 494
    TX_PLACEHOLDER(TX_ACTIONS_SLASH_DAGGER_B,                       4, 4),  // 495
    TX_PLACEHOLDER(TX_ACTIONS_GUARD_WOODEN_SHIELD_FRONT,            4, 4),  // 496
    TX_PLACEHOLDER(TX_ACTIONS_GUARD_KITE_SHIELD_SUCCESS_FRONT,      4, 4),  // 497
    TX_PLACEHOLDER(TX_ACTIONS_SHOT_BOW_B,                           4, 4),  // 498
    TX_PLACEHOLDER(TX_ACTIONS_SLASH_AXE_F,                          4, 4),  // 499
    TX_PLACEHOLDER(TX_ACTIONS_GUARD_KITE_SHIELD_SUCCESS_BACK,       4, 4),  // 500
};

/* ------------------------------------------------------------------ */
/*  Clean up macros (don't leak into other TUs)                        */
/* ------------------------------------------------------------------ */

#undef TX_FILE
#undef TX_PLACEHOLDER
#undef TX_TILEMAP

#undef GFX_CREATURES
#undef GFX_HUMAN
#undef GFX_HUMAN_SH
#undef GFX_ORC
#undef GFX_ORC_SH
#undef GFX_ELF
#undef GFX_ELF_SH
#undef GFX_DWARF
#undef GFX_DWARF_SH
#undef GFX_DWARF2
#undef GFX_DWARF2_SH
#undef GFX_HALFLING
#undef GFX_HALFLING_SH
#undef GFX_GOBLIN
#undef GFX_GOBLIN_SH
#undef GFX_WOLF
#undef GFX_WOLF_SH
#undef GFX_BAT
#undef GFX_BAT_SH
#undef GFX_WARG
#undef GFX_WARG_SH
#undef GFX_GSLIME
#undef GFX_GSLIME_SH
#undef GFX_SKELETON
#undef GFX_SKELETON_SH
#undef GFX_ZOMBIE
#undef GFX_ZOMBIE_SH
#undef GFX_DUNGEON_TS
