#pragma once

#include "item.h"
#include "potion.h"
#include "shield.h"
#include "weapon.h"
#include "sprite_def.h"

struct ItemDefinition {
    const char* id;
    itemtype_t category;
    weapontype_t weapon_type;
    shieldtype_t shield_type;
    potiontype_t potion_type;
    const char* name;
    const char* description;
    const rpg::SpriteDef* sprites;
    int sprite_count;
};

const ItemDefinition* find_weapon_definition(weapontype_t type);
const ItemDefinition* find_shield_definition(shieldtype_t type);
const ItemDefinition* find_potion_definition(potiontype_t type);
