/** @file inventory_renderer.h
 *  @brief Inventory, chest transfer, and item detail rendering.
 */

#pragma once

#include "gamestate.h"
#include "libdraw_context.h"
#include "spritegroup.h"
#include <algorithm>

namespace rpg
{

class InventoryRenderer {
public:
    void draw_inventory(gamestate& g, rpg::Renderer& renderer);
    void draw_chest(gamestate& g, rpg::Renderer& renderer);
    void draw_mini_inventory(gamestate& g, rpg::Renderer& renderer, const vector<entityid>& inventory, const char* title, const char* hint, bool show_equipped);

private:
    void draw_item_detail_panel(gamestate& g, rpg::Renderer& renderer, const Rectangle& right_box, entityid selection_id);
    void draw_inventory_grid(gamestate& g, rpg::Renderer& renderer, const vector<entityid>& inventory, const Rectangle& left_box, bool show_equipped_labels);
    Rectangle panel_for_hero(gamestate& g, float width, float height);
    vector<string> build_item_detail_lines(gamestate& g, entityid selection_id);
};

} // namespace rpg
