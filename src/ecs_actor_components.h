#pragma once

#include "race.h"
#include "sprite_def.h"
#include <string>

struct ActorKind {
    race_t race;
};

struct ActorVisual {
    const rpg::SpriteDef* sprites;
    int sprite_count;
};

struct ActorText {
    std::string default_name;
    std::string default_description;
};
