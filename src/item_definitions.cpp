#include "item_definitions.h"

#include "tx_keys_potions.h"
#include "tx_keys_shields.h"
#include "tx_keys_weapons.h"

namespace {
const ItemDefinition kWeaponDefinitions[] = {
    {
        "weapon.dagger",
        ITEM_WEAPON,
        WEAPON_DAGGER,
        SHIELD_NONE,
        POTION_NONE,
        "dagger",
        "Stabby stabby.",
        TX_DAGGER_KEYS,
        TX_DAGGER_COUNT,
    },
    {
        "weapon.short_sword",
        ITEM_WEAPON,
        WEAPON_SHORT_SWORD,
        SHIELD_NONE,
        POTION_NONE,
        "short sword",
        "your basic soldier's short sword",
        TX_SHORT_SWORD_KEYS,
        TX_SHORT_SWORD_COUNT,
    },
    {
        "weapon.axe",
        ITEM_WEAPON,
        WEAPON_AXE,
        SHIELD_NONE,
        POTION_NONE,
        "axe",
        "Choppy choppy",
        TX_AXE_KEYS,
        TX_AXE_COUNT,
    },
};

const ItemDefinition kShieldDefinitions[] = {
    {
        "shield.buckler",
        ITEM_SHIELD,
        WEAPON_NONE,
        SHIELD_BUCKLER,
        POTION_NONE,
        "buckler",
        "A light shield built for quick parries and close defense.",
        TX_BUCKLER_KEYS,
        TX_BUCKLER_COUNT,
    },
    {
        "shield.kite",
        ITEM_SHIELD,
        WEAPON_NONE,
        SHIELD_KITE,
        POTION_NONE,
        "kite shield",
        "Standard knight's shield",
        TX_KITE_SHIELD_KEYS,
        TX_KITE_SHIELD_COUNT,
    },
    {
        "shield.tower",
        ITEM_SHIELD,
        WEAPON_NONE,
        SHIELD_TOWER,
        POTION_NONE,
        "tower shield",
        "A tall shield meant to soak up punishment behind a wall of wood.",
        TX_TOWER_SHIELD_KEYS,
        TX_TOWER_SHIELD_COUNT,
    },
};

const ItemDefinition kPotionDefinitions[] = {
    {
        "potion.hp.small",
        ITEM_POTION,
        WEAPON_NONE,
        SHIELD_NONE,
        POTION_HP_SMALL,
        "small healing potion",
        "a small healing potion",
        TX_POTION_HP_SMALL_KEYS,
        TX_POTION_HP_SMALL_COUNT,
    },
    {
        "potion.hp.medium",
        ITEM_POTION,
        WEAPON_NONE,
        SHIELD_NONE,
        POTION_HP_MEDIUM,
        "medium healing potion",
        "a medium healing potion",
        TX_POTION_HP_MEDIUM_KEYS,
        TX_POTION_HP_MEDIUM_COUNT,
    },
    {
        "potion.hp.large",
        ITEM_POTION,
        WEAPON_NONE,
        SHIELD_NONE,
        POTION_HP_LARGE,
        "large healing potion",
        "a large healing potion",
        TX_POTION_HP_LARGE_KEYS,
        TX_POTION_HP_LARGE_COUNT,
    },
    {
        "potion.mp.small",
        ITEM_POTION,
        WEAPON_NONE,
        SHIELD_NONE,
        POTION_MP_SMALL,
        "small mana potion",
        "a small mana potion",
        TX_POTION_MP_SMALL_KEYS,
        TX_POTION_MP_SMALL_COUNT,
    },
    {
        "potion.mp.medium",
        ITEM_POTION,
        WEAPON_NONE,
        SHIELD_NONE,
        POTION_MP_MEDIUM,
        "medium mana potion",
        "a medium mana potion",
        TX_POTION_MP_MEDIUM_KEYS,
        TX_POTION_MP_MEDIUM_COUNT,
    },
    {
        "potion.mp.large",
        ITEM_POTION,
        WEAPON_NONE,
        SHIELD_NONE,
        POTION_MP_LARGE,
        "large mana potion",
        "a large mana potion",
        TX_POTION_MP_LARGE_KEYS,
        TX_POTION_MP_LARGE_COUNT,
    },
};
}

const ItemDefinition* find_weapon_definition(weapontype_t type) {
    for (const ItemDefinition& definition : kWeaponDefinitions) {
        if (definition.weapon_type == type) {
            return &definition;
        }
    }
    return nullptr;
}

const ItemDefinition* find_shield_definition(shieldtype_t type) {
    for (const ItemDefinition& definition : kShieldDefinitions) {
        if (definition.shield_type == type) {
            return &definition;
        }
    }
    return nullptr;
}

const ItemDefinition* find_potion_definition(potiontype_t type) {
    for (const ItemDefinition& definition : kPotionDefinitions) {
        if (definition.potion_type == type) {
            return &definition;
        }
    }
    return nullptr;
}
