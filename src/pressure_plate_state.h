#pragma once

#include "entityid.h"
#include "vec3.h"
#include <vector>

struct floor_pressure_plate_t {
    vec3 loc;
    entityid linked_door_id;
    bool active;
    bool destroyed;
    int txkey_up;
    int txkey_down;
};

namespace rpg {

struct PressurePlateState {
    std::vector<floor_pressure_plate_t> floor_pressure_plates;
    vec3 floor_four_tutorial_orc_spawn = vec3{-1, -1, -1};
};

} // namespace rpg
