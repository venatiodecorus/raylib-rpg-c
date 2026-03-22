/** @file world_renderer.h
 *  @brief World-space rendering: dungeon floor tiles, pressure plates, entities, damage numbers.
 */

#pragma once

#include "gamestate.h"
#include "libdraw_context.h"

#include <functional>

namespace rpg
{

class WorldRenderer {
public:
    /** @brief Draw the current floor's tiles plus entities in renderer layering order. */
    bool draw_floor(gamestate& g, rpg::Renderer& renderer, int vision_dist, int light_rad);

    /** @brief Draw all active floating damage-number popups for the current floor. */
    void draw_damage_numbers(gamestate& g);

private:
    /** @brief Draw the highlighted tile box in front of the player actor. */
    bool draw_player_target_box(gamestate& g, rpg::Renderer& renderer);

    /** @brief Draw one dungeon floor tile using its cached visibility and texture id. */
    bool draw_floor_tile(gamestate& g, rpg::Renderer& renderer, int x, int y, int z, int light_dist, vec3 hero_loc, int distance);

    /** @brief Draw floor pressure plates as overlays after floor tiles but before all entities. */
    void draw_floor_pressure_plates(gamestate& g, rpg::Renderer& renderer, int light_rad);

    /** @brief Draw one entity category across the current floor subject to visibility checks. */
    void draw_floor_entitytype(
        gamestate& g,
        rpg::Renderer& renderer,
        entitytype_t type_0,
        int vision_dist,
        int light_rad,
        const std::function<bool(gamestate&, entityid)>& extra_check);
};

} // namespace rpg
