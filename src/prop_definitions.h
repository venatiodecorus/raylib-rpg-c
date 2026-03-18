#pragma once

#include "proptype.h"

struct PropDefinition {
    const char* id;
    proptype_t type;
    const char* name;
    const char* description;
    bool solid;
    bool pushable;
    bool pullable;
    const int* sprite_keys;
    int sprite_key_count;
};

const PropDefinition& get_prop_definition(proptype_t type);
