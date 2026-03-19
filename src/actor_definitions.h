#pragma once

#include "race.h"
#include "alignment.h"
#include "sprite_def.h"
#include <string>

struct ActorDefinition {
    std::string id;
    race_t race;
    std::string default_name;
    std::string default_description;
    alignment_t default_alignment;
    int default_hitdie;
    int modifiers[6]; // str, dex, int, wis, con, cha
    const rpg::SpriteDef* sprites;
    int sprite_count;
};

const ActorDefinition* get_actor_definition(race_t race);
