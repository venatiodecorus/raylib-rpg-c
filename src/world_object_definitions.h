#pragma once

#include "entitytype.h"
#include "proptype.h"
#include "sprite_def.h"

struct StaticWorldDefinition {
    const char* id;
    entitytype_t legacy_type;
    proptype_t type;
    const char* name;
    const char* description;
    bool solid;
    bool pushable;
    bool pullable;
    bool openable;
    bool has_inventory;
    const rpg::SpriteDef* sprites;
    int sprite_count;
};

const StaticWorldDefinition& get_static_world_definition(entitytype_t legacy_type, proptype_t prop_type = PROP_NONE);
const StaticWorldDefinition& get_prop_definition(proptype_t type);
