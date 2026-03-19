#pragma once

#include "entitytype.h"
#include "proptype.h"
#include "sprite_def.h"
#include "vec3.h"

#include <string>

struct DefinitionRef {
    std::string id;
};

struct StaticVisual {
    const rpg::SpriteDef* sprites;
    int sprite_count;
};

struct InteractableText {
    std::string name;
    std::string description;
};

struct SolidTag {
    bool value;
};

struct PushableTag {
    bool value;
};

struct PullableTag {
    bool value;
};

struct GridPosition {
    vec3 loc;
};

struct LegacyEntityType {
    entitytype_t value;
};

struct PropKind {
    proptype_t type;
};

struct OpenState {
    bool value;
};

struct HasInventoryTag {
    bool value;
};
