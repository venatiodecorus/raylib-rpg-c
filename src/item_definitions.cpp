#include "item_definitions.h"

const rpg::SpriteDef tx_buckler_keys_sprites[] = {
    {rpg::TXSRC_PLACEHOLDER, "", 1, 1, 0, 0, 32, 32}, // TX_SHIELD_BUCKLER
    {rpg::TXSRC_PLACEHOLDER, "", 4, 4, 0, 0, 128, 128}, // TX_ACTIONS_GUARD_BUCKLER_SUCCESS_FRONT
    {rpg::TXSRC_PLACEHOLDER, "", 4, 4, 0, 0, 128, 128}, // TX_ACTIONS_GUARD_BUCKLER_SUCCESS_BACK
};

const rpg::SpriteDef tx_kite_shield_keys_sprites[] = {
    {rpg::TXSRC_PLACEHOLDER, "", 1, 1, 0, 0, 32, 32}, // TX_SHIELD_KITE_SHIELD
    {rpg::TXSRC_PLACEHOLDER, "", 4, 4, 0, 0, 128, 128}, // TX_ACTIONS_GUARD_KITE_SHIELD_SUCCESS_FRONT
    {rpg::TXSRC_PLACEHOLDER, "", 4, 4, 0, 0, 128, 128}, // TX_ACTIONS_GUARD_KITE_SHIELD_SUCCESS_BACK
};

const rpg::SpriteDef tx_tower_shield_keys_sprites[] = {
    {rpg::TXSRC_PLACEHOLDER, "", 1, 1, 0, 0, 32, 32}, // TX_SHIELD_WOODEN_TOWER_SHIELD
    {rpg::TXSRC_PLACEHOLDER, "", 4, 4, 0, 0, 128, 128}, // TX_ACTIONS_GUARD_TOWER_SHIELD_SUCCESS_FRONT
    {rpg::TXSRC_PLACEHOLDER, "", 4, 4, 0, 0, 128, 128}, // TX_ACTIONS_GUARD_TOWER_SHIELD_SUCCESS_BACK
};

const rpg::SpriteDef tx_potion_hp_small_keys_sprites[] = {
    {rpg::TXSRC_PLACEHOLDER, "", 1, 1, 0, 0, 32, 32}, // TX_ITEM_POTION_HP_SMALL
};

const rpg::SpriteDef tx_potion_hp_medium_keys_sprites[] = {
    {rpg::TXSRC_PLACEHOLDER, "", 1, 1, 0, 0, 32, 32}, // TX_ITEM_POTION_HP_MEDIUM
};

const rpg::SpriteDef tx_potion_hp_large_keys_sprites[] = {
    {rpg::TXSRC_PLACEHOLDER, "", 1, 1, 0, 0, 32, 32}, // TX_ITEM_POTION_HP_LARGE
};

const rpg::SpriteDef tx_potion_mp_small_keys_sprites[] = {
    {rpg::TXSRC_PLACEHOLDER, "", 1, 1, 0, 0, 32, 32}, // TX_ITEM_POTION_MP_SMALL
};

const rpg::SpriteDef tx_potion_mp_medium_keys_sprites[] = {
    {rpg::TXSRC_PLACEHOLDER, "", 1, 1, 0, 0, 32, 32}, // TX_ITEM_POTION_MP_MEDIUM
};

const rpg::SpriteDef tx_potion_mp_large_keys_sprites[] = {
    {rpg::TXSRC_PLACEHOLDER, "", 1, 1, 0, 0, 32, 32}, // TX_ITEM_POTION_MP_LARGE
};

const rpg::SpriteDef tx_short_sword_keys_sprites[] = {
    {rpg::TXSRC_PLACEHOLDER, "", 1, 1, 0, 0, 32, 32}, // TX_WEAPON_SHORT_SWORD
    {rpg::TXSRC_PLACEHOLDER, "", 4, 4, 0, 0, 128, 128}, // TX_ACTIONS_SLASH_SHORT_SWORD_F
    {rpg::TXSRC_PLACEHOLDER, "", 4, 4, 0, 0, 128, 128}, // TX_ACTIONS_SLASH_SHORT_SWORD_B
};

const rpg::SpriteDef tx_dagger_keys_sprites[] = {
    {rpg::TXSRC_PLACEHOLDER, "", 1, 1, 0, 0, 32, 32}, // TX_WEAPON_DAGGER
    {rpg::TXSRC_PLACEHOLDER, "", 4, 4, 0, 0, 128, 128}, // TX_ACTIONS_SLASH_DAGGER_F
    {rpg::TXSRC_PLACEHOLDER, "", 4, 4, 0, 0, 128, 128}, // TX_ACTIONS_SLASH_DAGGER_B
};

const rpg::SpriteDef tx_axe_keys_sprites[] = {
    {rpg::TXSRC_PLACEHOLDER, "", 1, 1, 0, 0, 32, 32}, // TX_WEAPON_AXE
    {rpg::TXSRC_PLACEHOLDER, "", 4, 4, 0, 0, 128, 128}, // TX_ACTIONS_SLASH_AXE_F
    {rpg::TXSRC_PLACEHOLDER, "", 4, 4, 0, 0, 128, 128}, // TX_ACTIONS_SLASH_AXE_B
};



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
        tx_dagger_keys_sprites,
        3,
    },
    {
        "weapon.short_sword",
        ITEM_WEAPON,
        WEAPON_SHORT_SWORD,
        SHIELD_NONE,
        POTION_NONE,
        "short sword",
        "your basic soldier's short sword",
        tx_short_sword_keys_sprites,
        3,
    },
    {
        "weapon.axe",
        ITEM_WEAPON,
        WEAPON_AXE,
        SHIELD_NONE,
        POTION_NONE,
        "axe",
        "Choppy choppy",
        tx_axe_keys_sprites,
        3,
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
        tx_buckler_keys_sprites,
        3,
    },
    {
        "shield.kite",
        ITEM_SHIELD,
        WEAPON_NONE,
        SHIELD_KITE,
        POTION_NONE,
        "kite shield",
        "Standard knight's shield",
        tx_kite_shield_keys_sprites,
        3,
    },
    {
        "shield.tower",
        ITEM_SHIELD,
        WEAPON_NONE,
        SHIELD_TOWER,
        POTION_NONE,
        "tower shield",
        "A tall shield meant to soak up punishment behind a wall of wood.",
        tx_tower_shield_keys_sprites,
        3,
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
        tx_potion_hp_small_keys_sprites,
        1,
    },
    {
        "potion.hp.medium",
        ITEM_POTION,
        WEAPON_NONE,
        SHIELD_NONE,
        POTION_HP_MEDIUM,
        "medium healing potion",
        "a medium healing potion",
        tx_potion_hp_medium_keys_sprites,
        1,
    },
    {
        "potion.hp.large",
        ITEM_POTION,
        WEAPON_NONE,
        SHIELD_NONE,
        POTION_HP_LARGE,
        "large healing potion",
        "a large healing potion",
        tx_potion_hp_large_keys_sprites,
        1,
    },
    {
        "potion.mp.small",
        ITEM_POTION,
        WEAPON_NONE,
        SHIELD_NONE,
        POTION_MP_SMALL,
        "small mana potion",
        "a small mana potion",
        tx_potion_mp_small_keys_sprites,
        1,
    },
    {
        "potion.mp.medium",
        ITEM_POTION,
        WEAPON_NONE,
        SHIELD_NONE,
        POTION_MP_MEDIUM,
        "medium mana potion",
        "a medium mana potion",
        tx_potion_mp_medium_keys_sprites,
        1,
    },
    {
        "potion.mp.large",
        ITEM_POTION,
        WEAPON_NONE,
        SHIELD_NONE,
        POTION_MP_LARGE,
        "large mana potion",
        "a large mana potion",
        tx_potion_mp_large_keys_sprites,
        1,
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
