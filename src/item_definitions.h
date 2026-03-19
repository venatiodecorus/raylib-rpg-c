#pragma once

#include "item.h"
#include "potion.h"
#include "shield.h"
#include "weapon.h"

struct ItemDefinition {
    const char* id;
    itemtype_t category;
    weapontype_t weapon_type;
    shieldtype_t shield_type;
    potiontype_t potion_type;
    const char* name;
    const char* description;
    const int* sprite_keys;
    int sprite_key_count;
};

const ItemDefinition* find_weapon_definition(weapontype_t type);
const ItemDefinition* find_shield_definition(shieldtype_t type);
const ItemDefinition* find_potion_definition(potiontype_t type);
