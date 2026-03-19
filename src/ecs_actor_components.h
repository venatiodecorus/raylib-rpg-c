#pragma once

#include "race.h"
#include <string>

struct ActorKind {
    race_t race;
};

struct ActorVisual {
    const int* keys;
    int key_count;
};

struct ActorText {
    std::string default_name;
    std::string default_description;
};
