#include "draw_look_panel.h"

void draw_look_panel(gamestate& g) {
    auto loc = g.ct.get<location>(g.hero_id).value_or(vec3{-1, -1, -1});
    auto df = g.d.get_floor(loc.z);
    tile_t& tile = df->tile_at(loc);
    const int entity_count = tile.entity_count() - 1;
    const int texts_size = 7;
    const char* dummy_text = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    const int font_size = DEFAULT_LOOK_PANEL_FONT_SIZE;
    const int pad_w = 10;
    const int pad_h = 10;
    const int m = MeasureText(dummy_text, font_size);
    const float w = m + pad_w * 2;
    const float h = font_size * texts_size + pad_h * (texts_size + 2);
    const float base_x = 10;
    const float base_y = g.targetheight - h;
    Rectangle r = {base_x, base_y, w, h};

    DrawRectanglePro(r, Vector2{0, 0}, 0.0f, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(r, 1, g.ui.window_box_fgcolor);
    DrawText("Look Panel", r.x + pad_w, r.y + pad_h + (font_size + 5) * 0, font_size, g.ui.window_box_fgcolor);
    DrawText(TextFormat("There are %d things here", entity_count), r.x + pad_w, r.y + pad_h + (font_size + 5) * 1, font_size, g.ui.window_box_fgcolor);
    DrawText(TextFormat("living npc %d", tile.get_cached_live_npc()), r.x + pad_w, r.y + pad_h + (font_size + 5) * 2, font_size, g.ui.window_box_fgcolor);
    DrawText(TextFormat("dead npc %d", tile.get_cached_dead_npc()), r.x + pad_w, r.y + pad_h + (font_size + 5) * 3, font_size, g.ui.window_box_fgcolor);
    DrawText(TextFormat("items %d (%d)", static_cast<int>(tile.get_item_count()), tile.get_cached_item()), r.x + pad_w, r.y + pad_h + (font_size + 5) * 4, font_size, g.ui.window_box_fgcolor);
    DrawText(TextFormat("door %d", tile.get_cached_door()), r.x + pad_w, r.y + pad_h + (font_size + 5) * 5, font_size, g.ui.window_box_fgcolor);
    DrawText(TextFormat("box %d", tile.get_cached_box()), r.x + pad_w, r.y + pad_h + (font_size + 5) * 6, font_size, g.ui.window_box_fgcolor);
}
