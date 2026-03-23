/** @file damage_popups.cpp
 *  @brief Floating damage number popup subsystem implementation.
 */

#include "damage_popups.h"

namespace rpg {

void DamagePopups::add(int x, int y, int floor, int amount, bool critical, std::mt19937& rng) {
    if (amount <= 0) {
        return;
    }

    const float anchor_x = x * DEFAULT_TILE_SIZE + (DEFAULT_TILE_SIZE * 0.5f);
    const size_t active_on_target = std::count_if(
        popups.begin(),
        popups.end(),
        [anchor_x, floor](const damage_popup_t& popup) {
            return popup.world_anchor.x == anchor_x
                && popup.floor == floor;
        });

    std::uniform_real_distribution<float> drift_dist(-2.5f, 2.5f);
    damage_popup_t popup = {};
    popup.target_id = ENTITYID_INVALID;
    popup.amount = amount;
    popup.critical = critical;
    popup.floor = floor;
    popup.world_anchor = Vector2{
        x * DEFAULT_TILE_SIZE + (DEFAULT_TILE_SIZE * 0.5f),
        y * DEFAULT_TILE_SIZE - 4.0f - static_cast<float>(active_on_target) * 2.0f,
    };
    popup.age_seconds = 0.0f;
    popup.lifetime_seconds = 0.7f;
    popup.drift_x = drift_dist(rng);
    popup.rise_distance = 10.0f + static_cast<float>(active_on_target);

    constexpr size_t max_damage_popups = 48;
    if (popups.size() >= max_damage_popups) {
        popups.erase(popups.begin());
    }
    popups.push_back(popup);
    dirty = true;
}

void DamagePopups::update(float dt_seconds) {
    if (popups.empty()) {
        return;
    }

    const float dt = std::max(0.0f, dt_seconds);
    for (damage_popup_t& popup : popups) {
        popup.age_seconds += dt;
    }

    popups.erase(
        std::remove_if(
            popups.begin(),
            popups.end(),
            [](const damage_popup_t& popup) { return popup.age_seconds >= popup.lifetime_seconds; }),
        popups.end());

    dirty = true;
}

} // namespace rpg
