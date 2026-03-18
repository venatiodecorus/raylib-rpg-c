#pragma once

#include "proptype.h"
#include "vec3.h"

#include <string>

struct DefinitionRef {
    std::string id;
};

struct PropVisual {
    const int* keys;
    int key_count;
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

struct PropKind {
    proptype_t type;
};
