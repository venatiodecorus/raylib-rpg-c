/** @file damage_popups.h
 *  @brief Floating damage number popup subsystem.
 */
#pragma once

#include "entityid.h"
#include "libgame_defines.h"
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <random>
#include <algorithm>

struct damage_popup_t {
    entityid target_id = -1;
    int amount;
    bool critical;
    int floor;
    Vector2 world_anchor;
    float age_seconds;
    float lifetime_seconds;
    float drift_x;
    float rise_distance;
};

namespace rpg {

class DamagePopups {
public:
    std::vector<damage_popup_t> popups;
    bool dirty = false;

    void add(int x, int y, int floor, int amount, bool critical, std::mt19937& rng);
    void update(float dt_seconds);
};

} // namespace rpg
