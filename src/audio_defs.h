/** @file audio_defs.h
 *  @brief Code-defined audio asset paths for SFX and music.
 *
 *  Replaces the old runtime-parsed sfx.txt and music.txt files.
 *  All paths are relative to the project root.
 */

#pragma once

/* ------------------------------------------------------------------ */
/*  SFX base path                                                      */
/* ------------------------------------------------------------------ */

#define SFX_BASE "sfx/Minifantasy_Dungeon_SFX/"

/* ------------------------------------------------------------------ */
/*  SFX definitions — 62 sound effects                                 */
/*  Index order must match SFX_* defines in sfx.h                      */
/* ------------------------------------------------------------------ */

#define SFX_DEF_COUNT 62

static const char* SFX_PATHS[SFX_DEF_COUNT] = {
    SFX_BASE "01_chest_open_1.wav",          //  0  SFX_CHEST_OPEN_1
    SFX_BASE "01_chest_open_2.wav",          //  1  SFX_CHEST_OPEN_2
    SFX_BASE "01_chest_open_3.wav",          //  2  SFX_CHEST_OPEN_3
    SFX_BASE "01_chest_open_4.wav",          //  3  SFX_CHEST_OPEN_4
    SFX_BASE "02_chest_close_1.wav",         //  4  SFX_CHEST_CLOSE_1
    SFX_BASE "02_chest_close_2.wav",         //  5  SFX_CHEST_CLOSE_2
    SFX_BASE "02_chest_close_3.wav",         //  6  SFX_CHEST_CLOSE_3
    SFX_BASE "03_crate_open_1.wav",          //  7  SFX_CRATE_OPEN_1
    SFX_BASE "03_crate_open_2.wav",          //  8  SFX_CRATE_OPEN_2
    SFX_BASE "03_crate_open_3.wav",          //  9  SFX_CRATE_OPEN_3
    SFX_BASE "04_sack_open_1.wav",           // 10  SFX_SACK_OPEN_1
    SFX_BASE "04_sack_open_2.wav",           // 11  SFX_SACK_OPEN_2
    SFX_BASE "04_sack_open_3.wav",           // 12  SFX_SACK_OPEN_3
    SFX_BASE "05_door_open_1.mp3",           // 13  SFX_DOOR_OPEN_1
    SFX_BASE "05_door_open_2.mp3",           // 14  SFX_DOOR_OPEN_2
    SFX_BASE "06_door_close_1.mp3",          // 15  SFX_DOOR_CLOSE_1
    SFX_BASE "06_door_close_2.mp3",          // 16  SFX_DOOR_CLOSE_2
    SFX_BASE "07_human_atk_sword_1.wav",     // 17  SFX_HUMAN_ATK_SWORD_1
    SFX_BASE "07_human_atk_sword_2.wav",     // 18  SFX_HUMAN_ATK_SWORD_2
    SFX_BASE "07_human_atk_sword_3.wav",     // 19  SFX_HUMAN_ATK_SWORD_3
    SFX_BASE "08_human_charge_1.wav",        // 20  SFX_HUMAN_CHARGE_1
    SFX_BASE "08_human_charge_2.wav",        // 21  SFX_HUMAN_CHARGE_2
    SFX_BASE "09_human_charging_1_loop.wav", // 22  SFX_HUMAN_CHARGING_1_LOOP
    SFX_BASE "09_human_charging_2_loop.wav", // 23  SFX_HUMAN_CHARGING_2_LOOP
    SFX_BASE "10_human_special_atk_1.wav",   // 24  SFX_HUMAN_SPECIAL_ATK_1
    SFX_BASE "10_human_special_atk_2.wav",   // 25  SFX_HUMAN_SPECIAL_ATK_2
    SFX_BASE "11_human_damage_1.wav",        // 26  SFX_HUMAN_DAMAGE_1
    SFX_BASE "11_human_damage_2.wav",        // 27  SFX_HUMAN_DAMAGE_2
    SFX_BASE "11_human_damage_3.wav",        // 28  SFX_HUMAN_DAMAGE_3
    SFX_BASE "12_human_jump_1.wav",          // 29  SFX_HUMAN_JUMP_1
    SFX_BASE "12_human_jump_2.wav",          // 30  SFX_HUMAN_JUMP_2
    SFX_BASE "12_human_jump_3.wav",          // 31  SFX_HUMAN_JUMP_3
    SFX_BASE "13_human_jump_land_1.wav",     // 32  SFX_HUMAN_JUMP_LAND_1
    SFX_BASE "13_human_jump_land_2.wav",     // 33  SFX_HUMAN_JUMP_LAND_2
    SFX_BASE "14_human_death_spin.wav",      // 34  SFX_HUMAN_DEATH_SPIN
    SFX_BASE "15_human_dash_1.wav",          // 35  SFX_HUMAN_DASH_1
    SFX_BASE "15_human_dash_2.wav",          // 36  SFX_HUMAN_DASH_2
    SFX_BASE "16_human_walk_stone_1.wav",    // 37  SFX_HUMAN_WALK_STONE_1
    SFX_BASE "16_human_walk_stone_2.wav",    // 38  SFX_HUMAN_WALK_STONE_2
    SFX_BASE "16_human_walk_stone_3.wav",    // 39  SFX_HUMAN_WALK_STONE_3
    SFX_BASE "17_orc_atk_sword_1.wav",      // 40  SFX_ORC_ATK_SWORD_1
    SFX_BASE "17_orc_atk_sword_2.wav",      // 41  SFX_ORC_ATK_SWORD_2
    SFX_BASE "17_orc_atk_sword_3.wav",      // 42  SFX_ORC_ATK_SWORD_3
    SFX_BASE "18_orc_charge.wav",            // 43  SFX_ORC_CHARGE
    SFX_BASE "19_orc_charging_loop.wav",     // 44  SFX_ORC_CHARGING_LOOP
    SFX_BASE "20_orc_special_atk.wav",       // 45  SFX_ORC_SPECIAL_ATK
    SFX_BASE "21_orc_damage_1.wav",          // 46  SFX_ORC_DAMAGE_1
    SFX_BASE "21_orc_damage_2.wav",          // 47  SFX_ORC_DAMAGE_2
    SFX_BASE "21_orc_damage_3.wav",          // 48  SFX_ORC_DAMAGE_3
    SFX_BASE "22_orc_jump_1.wav",            // 49  SFX_ORC_JUMP_1
    SFX_BASE "22_orc_jump_2.wav",            // 50  SFX_ORC_JUMP_2
    SFX_BASE "23_orc_jump_land.wav",         // 51  SFX_ORC_JUMP_LAND
    SFX_BASE "24_orc_death_spin.wav",        // 52  SFX_ORC_DEATH_SPIN
    SFX_BASE "25_orc_walk_stone_1.wav",      // 53  SFX_ORC_WALK_STONE_1
    SFX_BASE "25_orc_walk_stone_2.wav",      // 54  SFX_ORC_WALK_STONE_2
    SFX_BASE "25_orc_walk_stone_3.wav",      // 55  SFX_ORC_WALK_STONE_3
    SFX_BASE "26_sword_hit_1.wav",           // 56  SFX_SWORD_HIT_1
    SFX_BASE "26_sword_hit_2.wav",           // 57  SFX_SWORD_HIT_2
    SFX_BASE "26_sword_hit_3.wav",           // 58  SFX_SWORD_HIT_3
    SFX_BASE "27_sword_miss_1.wav",          // 59  SFX_SWORD_MISS_1
    SFX_BASE "27_sword_miss_2.wav",          // 60  SFX_SWORD_MISS_2
    SFX_BASE "27_sword_miss_3.wav",          // 61  SFX_SWORD_MISS_3
};

/* ------------------------------------------------------------------ */
/*  Music definitions — 2 tracks                                       */
/* ------------------------------------------------------------------ */

#define MUSIC_BASE "sfx/Minifantasy_Dungeon_Music/Music/"

#define MUSIC_DEF_COUNT 2

static const char* MUSIC_PATHS[MUSIC_DEF_COUNT] = {
    MUSIC_BASE "Goblins_Dance_(Battle).wav",
    MUSIC_BASE "Goblins_Den_(Regular).wav",
};
