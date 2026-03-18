#include "draw_inventory_menu.h"

#include "draw_chest_menu.h"

void draw_inventory_menu(gamestate& g) {
    if (!g.ui.display_inventory_menu) {
        return;
    }

    const char* menu_title = "Inventory Menu";
    const int section_gap = 8;
    const int font_size = 10;
    const Vector2 title_size = MeasureTextEx(GetFontDefault(), menu_title, font_size, g.line_spacing);
    const float menu_width_percent = 0.75f;
    const float menu_height_percent = 0.75f;
    const int w = DEFAULT_TARGET_WIDTH;
    const int h = DEFAULT_TARGET_HEIGHT;
    const float menu_width = w * menu_width_percent;
    const float menu_height = h * menu_height_percent;
    Rectangle menu_box = {(w - menu_width) / 2.0f, (h - menu_height) / 4.0f, menu_width, menu_height};
    float title_x = menu_box.x + (menu_box.width - title_size.x) / 2.0f;
    float title_y = menu_box.y + g.pad;
    float half_width = (menu_box.width - section_gap) / 2.0f;
    float half_height = menu_box.height - title_size.y - g.pad * 3.0f;

    DrawRectangleRec(menu_box, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(menu_box, 2, g.ui.window_box_fgcolor);
    DrawText(menu_title, title_x, title_y, font_size, g.ui.window_box_fgcolor);

    Rectangle left_box = {menu_box.x + g.pad, title_y + title_size.y + g.pad, half_width - g.pad, half_height};
    Rectangle right_box = {left_box.x + half_width + section_gap, left_box.y, half_width - g.pad * 2, half_height};
    DrawRectangleRec(left_box, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(left_box, 2, g.ui.window_box_fgcolor);
    DrawRectangleRec(right_box, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(right_box, 2, g.ui.window_box_fgcolor);

    auto maybe_inventory = g.ct.get<inventory>(g.hero_id);
    if (!maybe_inventory.has_value()) {
        return;
    }

    auto inventory = maybe_inventory.value();
    draw_inventory_grid(g, inventory, left_box, true);

    if (!inventory->empty()) {
        size_t index = static_cast<size_t>(g.ui.inventory_cursor.y) * 7 + static_cast<size_t>(g.ui.inventory_cursor.x);
        if (index < inventory->size()) {
            draw_item_detail_panel(g, right_box, inventory->at(index));
        }
    }
}
