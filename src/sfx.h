/** @file sfx.h
 *  @brief Integer sound-effect identifiers used to index loaded SFX assets.
 *
 *  IDs correspond to the SFX_PATHS[] array order in audio_defs.h.
 *  Legacy aliases map old SFX_* names to the closest available sound.
 */

#pragma once

/* ------------------------------------------------------------------ */
/*  Primary SFX IDs — match SFX_PATHS[] order in audio_defs.h          */
/* ------------------------------------------------------------------ */

// Chest open (01)
#define SFX_CHEST_OPEN_1               0
#define SFX_CHEST_OPEN_2               1
#define SFX_CHEST_OPEN_3               2
#define SFX_CHEST_OPEN_4               3

// Chest close (02)
#define SFX_CHEST_CLOSE_1              4
#define SFX_CHEST_CLOSE_2              5
#define SFX_CHEST_CLOSE_3              6

// Crate open (03)
#define SFX_CRATE_OPEN_1               7
#define SFX_CRATE_OPEN_2               8
#define SFX_CRATE_OPEN_3               9

// Sack open (04)
#define SFX_SACK_OPEN_1               10
#define SFX_SACK_OPEN_2               11
#define SFX_SACK_OPEN_3               12

// Door open/close (05-06)
#define SFX_DOOR_OPEN_1               13
#define SFX_DOOR_OPEN_2               14
#define SFX_DOOR_CLOSE_1              15
#define SFX_DOOR_CLOSE_2              16

// Human attack sword (07)
#define SFX_HUMAN_ATK_SWORD_1         17
#define SFX_HUMAN_ATK_SWORD_2         18
#define SFX_HUMAN_ATK_SWORD_3         19

// Human charge (08)
#define SFX_HUMAN_CHARGE_1            20
#define SFX_HUMAN_CHARGE_2            21

// Human charging loop (09)
#define SFX_HUMAN_CHARGING_1_LOOP     22
#define SFX_HUMAN_CHARGING_2_LOOP     23

// Human special attack (10)
#define SFX_HUMAN_SPECIAL_ATK_1       24
#define SFX_HUMAN_SPECIAL_ATK_2       25

// Human damage (11)
#define SFX_HUMAN_DAMAGE_1            26
#define SFX_HUMAN_DAMAGE_2            27
#define SFX_HUMAN_DAMAGE_3            28

// Human jump (12)
#define SFX_HUMAN_JUMP_1              29
#define SFX_HUMAN_JUMP_2              30
#define SFX_HUMAN_JUMP_3              31

// Human jump land (13)
#define SFX_HUMAN_JUMP_LAND_1         32
#define SFX_HUMAN_JUMP_LAND_2         33

// Human death spin (14)
#define SFX_HUMAN_DEATH_SPIN          34

// Human dash (15)
#define SFX_HUMAN_DASH_1              35
#define SFX_HUMAN_DASH_2              36

// Human walk stone (16)
#define SFX_HUMAN_WALK_STONE_1        37
#define SFX_HUMAN_WALK_STONE_2        38
#define SFX_HUMAN_WALK_STONE_3        39

// Orc attack sword (17)
#define SFX_ORC_ATK_SWORD_1           40
#define SFX_ORC_ATK_SWORD_2           41
#define SFX_ORC_ATK_SWORD_3           42

// Orc charge (18)
#define SFX_ORC_CHARGE                43

// Orc charging loop (19)
#define SFX_ORC_CHARGING_LOOP         44

// Orc special attack (20)
#define SFX_ORC_SPECIAL_ATK           45

// Orc damage (21)
#define SFX_ORC_DAMAGE_1              46
#define SFX_ORC_DAMAGE_2              47
#define SFX_ORC_DAMAGE_3              48

// Orc jump (22)
#define SFX_ORC_JUMP_1                49
#define SFX_ORC_JUMP_2                50

// Orc jump land (23)
#define SFX_ORC_JUMP_LAND             51

// Orc death spin (24)
#define SFX_ORC_DEATH_SPIN            52

// Orc walk stone (25)
#define SFX_ORC_WALK_STONE_1          53
#define SFX_ORC_WALK_STONE_2          54
#define SFX_ORC_WALK_STONE_3          55

// Sword hit (26)
#define SFX_SWORD_HIT_1               56
#define SFX_SWORD_HIT_2               57
#define SFX_SWORD_HIT_3               58

// Sword miss (27)
#define SFX_SWORD_MISS_1              59
#define SFX_SWORD_MISS_2              60
#define SFX_SWORD_MISS_3              61

/* ------------------------------------------------------------------ */
/*  Legacy aliases — map old SFX_* names to closest available sound    */
/* ------------------------------------------------------------------ */

// Alchemy sounds -> use crate open (glass-like)
#define SFX_01_ALCHEMY_LAB             SFX_CRATE_OPEN_1
#define SFX_02_ALCHEMY_GLASS_HANDLING_1 SFX_CRATE_OPEN_1
#define SFX_03_ALCHEMY_GLASS_HANDLING_2 SFX_CRATE_OPEN_2
#define SFX_04_ALCHEMY_GLASS_HANDLING_3 SFX_CRATE_OPEN_3
#define SFX_05_ALCHEMY_GLASS_BREAK     SFX_SWORD_HIT_1

// Bag sounds -> use sack/chest
#define SFX_BAG_CLOSE                  SFX_CHEST_CLOSE_1
#define SFX_BAG_FULL                   SFX_SACK_OPEN_1
#define SFX_BAG_OPEN                   SFX_SACK_OPEN_2

// Chest open -> chest open
#define SFX_CHEST_OPEN                 SFX_CHEST_OPEN_1

// Coins -> use crate open
#define SFX_COINS                      SFX_CRATE_OPEN_1

// Confirm -> use chest close (short click-like)
#define SFX_CONFIRM_01                 SFX_CHEST_CLOSE_1

// Discard -> use sword miss (whoosh)
#define SFX_DISCARD_ITEM               SFX_SWORD_MISS_1
#define SFX_DROP_WHOOSH                SFX_SWORD_MISS_2

// Equip -> use human charge (gear sound)
#define SFX_EQUIP_01                   SFX_HUMAN_CHARGE_1
#define SFX_EQUIP                      SFX_HUMAN_CHARGE_2

// Hit sounds -> use sword hit / damage
#define SFX_GET_HIT                    SFX_HUMAN_DAMAGE_1
#define SFX_HIT_METAL_ON_FLESH         SFX_SWORD_HIT_1
#define SFX_HIT_METAL_ON_LEATHER       SFX_SWORD_HIT_2
#define SFX_HIT_METAL_ON_METAL         SFX_SWORD_HIT_3
#define SFX_HIT_METAL_ON_STONE         SFX_SWORD_HIT_1
#define SFX_HIT_METAL_ON_WOOD_HEAVY    SFX_SWORD_HIT_2
#define SFX_HIT_METAL_ON_WOOD          SFX_SWORD_HIT_3
#define SFX_HIT_WET                    SFX_SWORD_HIT_1
#define SFX_HIT_WOOD_ON_FLESH          SFX_SWORD_HIT_2
#define SFX_HIT_WOOD_ON_LEATHER        SFX_SWORD_HIT_3
#define SFX_HIT_WOOD_ON_METAL          SFX_SWORD_HIT_1
#define SFX_HIT_WOOD_ON_STONE          SFX_SWORD_HIT_2
#define SFX_HIT_WOOD_ON_WOOD           SFX_SWORD_HIT_3

// Item sounds -> use sack/crate
#define SFX_IDENTIFY                   SFX_CRATE_OPEN_1
#define SFX_ITEM_FUSION                SFX_HUMAN_SPECIAL_ATK_1
#define SFX_ITEM_LOCK                  SFX_DOOR_CLOSE_1
#define SFX_ITEM_PICK                  SFX_SACK_OPEN_3
#define SFX_ITEM_PLACE                 SFX_SACK_OPEN_1
#define SFX_ITEM_UNLOCK                SFX_DOOR_OPEN_1
#define SFX_JEWEL                      SFX_CRATE_OPEN_2

// Jump -> use human jump
#define SFX_JUMP_1                     SFX_HUMAN_JUMP_1
#define SFX_JUMP_2                     SFX_HUMAN_JUMP_2
#define SFX_JUMP_3                     SFX_HUMAN_JUMP_3
#define SFX_LAND_1                     SFX_HUMAN_JUMP_LAND_1
#define SFX_LAND_2                     SFX_HUMAN_JUMP_LAND_2
#define SFX_LAND_ON_GRASS_1            SFX_HUMAN_JUMP_LAND_1
#define SFX_LAND_ON_GRASS_2            SFX_HUMAN_JUMP_LAND_2

// Material sounds -> use various
#define SFX_LEATHER                    SFX_SACK_OPEN_1
#define SFX_LEGENDARY_DROP             SFX_HUMAN_SPECIAL_ATK_2
#define SFX_METAL_HEAVY                SFX_SWORD_HIT_1
#define SFX_METAL_LIGHT                SFX_SWORD_HIT_2
#define SFX_PAGE_TURN                  SFX_SACK_OPEN_3
#define SFX_RARE_DROP                  SFX_HUMAN_SPECIAL_ATK_1
#define SFX_SCROLL                     SFX_SACK_OPEN_2

// Slash attacks -> use human/orc sword attacks
#define SFX_SLASH_ATTACK_HEAVY_1       SFX_HUMAN_ATK_SWORD_1
#define SFX_SLASH_ATTACK_HEAVY_2       SFX_HUMAN_ATK_SWORD_2
#define SFX_SLASH_ATTACK_HEAVY_3       SFX_HUMAN_ATK_SWORD_3
#define SFX_SLASH_ATTACK_LIGHT_1       SFX_HUMAN_ATK_SWORD_1
#define SFX_SLASH_ATTACK_LIGHT_2       SFX_HUMAN_ATK_SWORD_2
#define SFX_SLASH_ATTACK_LIGHT_3       SFX_HUMAN_ATK_SWORD_3
#define SFX_SLASH_ATTACK_SWORD_1       SFX_HUMAN_ATK_SWORD_1
#define SFX_SLASH_ATTACK_SWORD_2       SFX_HUMAN_ATK_SWORD_2
#define SFX_SLASH_ATTACK_SWORD_3       SFX_HUMAN_ATK_SWORD_3

// Socket sounds -> use equip
#define SFX_SOCKET_EQUIP               SFX_HUMAN_CHARGE_1
#define SFX_SOCKET_REMOVE              SFX_HUMAN_CHARGE_2
#define SFX_SORT_BAG                   SFX_SACK_OPEN_1

// Step sounds -> use human walk stone
#define SFX_STEP_DIRT_1                SFX_HUMAN_WALK_STONE_1
#define SFX_STEP_DIRT_2                SFX_HUMAN_WALK_STONE_2
#define SFX_STEP_DIRT_3                SFX_HUMAN_WALK_STONE_3
#define SFX_STEP_GRASS_1               SFX_HUMAN_WALK_STONE_1
#define SFX_STEP_GRASS_2               SFX_HUMAN_WALK_STONE_2
#define SFX_STEP_GRASS_3               SFX_HUMAN_WALK_STONE_3
#define SFX_STEP_STONE_1               SFX_HUMAN_WALK_STONE_1
#define SFX_STEP_STONE_2               SFX_HUMAN_WALK_STONE_2
#define SFX_STEP_STONE_3               SFX_HUMAN_WALK_STONE_3

// Unequip -> use chest close
#define SFX_UNEQUIP                    SFX_CHEST_CLOSE_2
