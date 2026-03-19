#pragma once

#include "race.h"
#include "alignment.h"
#include <string>

struct ActorDefinition {
    std::string id;
    race_t race;
    std::string default_name;
    std::string default_description;
    alignment_t default_alignment;
    int default_hitdie;
    int modifiers[6]; // str, dex, int, wis, con, cha
    const int* sprite_keys;
    int sprite_key_count;
};

const ActorDefinition* get_actor_definition(race_t race);
