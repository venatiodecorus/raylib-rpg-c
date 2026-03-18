#include "world_object_definitions.h"

#include "massert.h"
#include "tx_keys_boxes.h"
#include "tx_keys_chests.h"
#include "tx_keys_doors.h"
#include "tx_keys_props.h"

namespace {
const StaticWorldDefinition kPropDefinitions[] = {
    {
        "prop.none",
        ENTITY_PROP,
        PROP_NONE,
        "prop",
        "A bit of dungeon clutter left to rot in the dark.",
        false,
        false,
        false,
        false,
        false,
        nullptr,
        0,
    },
    {
        "prop.dungeon.banner_00",
        ENTITY_PROP,
        PROP_DUNGEON_BANNER_00,
        "banner",
        "A faded banner hanging limp in the stale dungeon air.",
        false,
        false,
        false,
        false,
        false,
        TX_PROP_DUNGEON_BANNER_00_KEYS,
        TX_PROP_DUNGEON_BANNER_00_COUNT,
    },
    {
        "prop.dungeon.banner_01",
        ENTITY_PROP,
        PROP_DUNGEON_BANNER_01,
        "banner",
        "A faded banner hanging limp in the stale dungeon air.",
        false,
        false,
        false,
        false,
        false,
        TX_PROP_DUNGEON_BANNER_01_KEYS,
        TX_PROP_DUNGEON_BANNER_01_COUNT,
    },
    {
        "prop.dungeon.banner_02",
        ENTITY_PROP,
        PROP_DUNGEON_BANNER_02,
        "banner",
        "A faded banner hanging limp in the stale dungeon air.",
        false,
        false,
        false,
        false,
        false,
        TX_PROP_DUNGEON_BANNER_02_KEYS,
        TX_PROP_DUNGEON_BANNER_02_COUNT,
    },
    {
        "prop.dungeon.statue_00",
        ENTITY_PROP,
        PROP_DUNGEON_STATUE_00,
        "statue",
        "A heavy carved statue worn smooth by years of damp air and passing hands.",
        true,
        true,
        false,
        false,
        false,
        TX_PROP_DUNGEON_STATUE_00_KEYS,
        TX_PROP_DUNGEON_STATUE_00_COUNT,
    },
    {
        "prop.dungeon.torch_00",
        ENTITY_PROP,
        PROP_DUNGEON_TORCH_00,
        "torch",
        "A soot-blackened torch stand dragged from the wall and left among the rubble.",
        false,
        false,
        false,
        false,
        false,
        TX_PROP_DUNGEON_TORCH_00_KEYS,
        TX_PROP_DUNGEON_TORCH_00_COUNT,
    },
    {
        "prop.dungeon.candle_00",
        ENTITY_PROP,
        PROP_DUNGEON_CANDLE_00,
        "candle",
        "A stubby candle with wax pooled around its base.",
        false,
        false,
        true,
        false,
        false,
        TX_PROP_DUNGEON_CANDLE_00_KEYS,
        TX_PROP_DUNGEON_CANDLE_00_COUNT,
    },
    {
        "prop.dungeon.jar_00",
        ENTITY_PROP,
        PROP_DUNGEON_JAR_00,
        "jar",
        "A ceramic jar with a chipped lip and a dusting of old clay inside.",
        false,
        false,
        false,
        false,
        false,
        TX_PROP_DUNGEON_JAR_00_KEYS,
        TX_PROP_DUNGEON_JAR_00_COUNT,
    },
    {
        "prop.dungeon.plate_00",
        ENTITY_PROP,
        PROP_DUNGEON_PLATE_00,
        "plate",
        "A dusty plate left behind by some long-finished meal.",
        false,
        false,
        false,
        false,
        false,
        TX_PROP_DUNGEON_PLATE_00_KEYS,
        TX_PROP_DUNGEON_PLATE_00_COUNT,
    },
    {
        "prop.dungeon.wooden_barrel_open_top_empty",
        ENTITY_PROP,
        PROP_DUNGEON_WOODEN_BARREL_OPEN_TOP_EMPTY,
        "empty barrel",
        "An open-topped barrel that smells faintly of stale ale.",
        true,
        false,
        false,
        false,
        false,
        TX_PROP_DUNGEON_WOODEN_BARREL_OPEN_TOP_EMPTY_KEYS,
        TX_PROP_DUNGEON_WOODEN_BARREL_OPEN_TOP_EMPTY_COUNT,
    },
    {
        "prop.dungeon.wooden_barrel_open_top_water",
        ENTITY_PROP,
        PROP_DUNGEON_WOODEN_BARREL_OPEN_TOP_WATER,
        "water barrel",
        "An open-topped barrel filled with still water dark enough to hide the bottom.",
        true,
        false,
        false,
        false,
        false,
        TX_PROP_DUNGEON_WOODEN_BARREL_OPEN_TOP_WATER_KEYS,
        TX_PROP_DUNGEON_WOODEN_BARREL_OPEN_TOP_WATER_COUNT,
    },
    {
        "prop.dungeon.wooden_chair_00",
        ENTITY_PROP,
        PROP_DUNGEON_WOODEN_CHAIR_00,
        "wooden chair",
        "A wooden chair with scraped legs and a backrest polished by use.",
        false,
        false,
        false,
        false,
        false,
        TX_PROP_DUNGEON_WOODEN_CHAIR_00_KEYS,
        TX_PROP_DUNGEON_WOODEN_CHAIR_00_COUNT,
    },
    {
        "prop.dungeon.wooden_table_00",
        ENTITY_PROP,
        PROP_DUNGEON_WOODEN_TABLE_00,
        "wooden table",
        "A sturdy wooden table scarred by cuts, heat marks, and years of hard use.",
        true,
        true,
        true,
        false,
        false,
        TX_PROP_DUNGEON_WOODEN_TABLE_00_KEYS,
        TX_PROP_DUNGEON_WOODEN_TABLE_00_COUNT,
    },
    {
        "prop.dungeon.wooden_table_01",
        ENTITY_PROP,
        PROP_DUNGEON_WOODEN_TABLE_01,
        "wooden table",
        "A sturdy wooden table scarred by cuts, heat marks, and years of hard use.",
        true,
        true,
        true,
        false,
        false,
        TX_PROP_DUNGEON_WOODEN_TABLE_01_KEYS,
        TX_PROP_DUNGEON_WOODEN_TABLE_01_COUNT,
    },
    {
        "prop.dungeon.wooden_sign",
        ENTITY_PROP,
        PROP_DUNGEON_WOODEN_SIGN,
        "wooden sign",
        "Pull something onto the\npressure plate to keep the door open.",
        true,
        false,
        false,
        false,
        false,
        TX_PROP_DUNGEON_WOODEN_SIGN_KEYS,
        TX_PROP_DUNGEON_WOODEN_SIGN_COUNT,
    },
};

const StaticWorldDefinition kWoodenDoorDefinition = {
    "door.wooden",
    ENTITY_DOOR,
    PROP_NONE,
    "door",
    "A heavy wooden door bound with iron straps and swollen from the dungeon damp.",
    false,
    false,
    false,
    true,
    false,
    TX_WOODEN_DOOR_KEYS,
    TX_WOODEN_DOOR_COUNT,
};

const StaticWorldDefinition kTreasureChestDefinition = {
    "chest.treasure",
    ENTITY_CHEST,
    PROP_NONE,
    "treasure chest",
    "A stout treasure chest reinforced with iron bands and built to survive rough handling.",
    true,
    true,
    true,
    true,
    true,
    TX_TREASURE_CHEST_KEYS,
    TX_TREASURE_CHEST_COUNT,
};

const StaticWorldDefinition kWoodenBoxDefinition = {
    "box.wooden",
    ENTITY_BOX,
    PROP_NONE,
    "box",
    "A plain wooden box with rough handles cut into the sides for hauling.",
    false,
    true,
    true,
    false,
    false,
    TX_WOODEN_BOX_KEYS,
    TX_WOODEN_BOX_COUNT,
};
}

const StaticWorldDefinition& get_static_world_definition(entitytype_t legacy_type, proptype_t prop_type) {
    switch (legacy_type) {
    case ENTITY_PROP: {
        const int index = static_cast<int>(prop_type);
        massert(index >= 0, "prop type is negative: %d", index);
        massert(index < static_cast<int>(sizeof(kPropDefinitions) / sizeof(kPropDefinitions[0])), "prop type is out of range: %d", index);
        massert(kPropDefinitions[index].type == prop_type, "prop definition mismatch at index %d", index);
        return kPropDefinitions[index];
    }
    case ENTITY_DOOR:
        return kWoodenDoorDefinition;
    case ENTITY_CHEST:
        return kTreasureChestDefinition;
    case ENTITY_BOX:
        return kWoodenBoxDefinition;
    default:
        break;
    }

    massert(false, "unsupported static world definition lookup for entity type %d", static_cast<int>(legacy_type));
    return kPropDefinitions[0];
}

const StaticWorldDefinition& get_prop_definition(proptype_t type) {
    return get_static_world_definition(ENTITY_PROP, type);
}
