#pragma once

#include "item.h"
#include "potion.h"
#include "shield.h"
#include "weapon.h"

#include <string>

struct ItemKind {
    itemtype_t type;
};

struct WeaponKind {
    weapontype_t type;
};

struct ShieldKind {
    shieldtype_t type;
};

struct PotionKind {
    potiontype_t type;
};

struct ItemVisual {
    const int* keys;
    int key_count;
};

struct ItemText {
    std::string name;
    std::string description;
};
