#include "draw_sound_menu.h"

void draw_sound_menu(gamestate& g) {
    if (!g.ui.display_sound_menu) {
        return;
    }

    constexpr int font_size = 12;
    constexpr int title_font_size = 18;
    constexpr int line_height = 16;
    constexpr int padding = 12;
    const int box_w = 360;
    const int box_h = 140;
    const int box_x = (DEFAULT_TARGET_WIDTH - box_w) / 2;
    const int box_y = (DEFAULT_TARGET_HEIGHT - box_h) / 2;
    const Rectangle box = {static_cast<float>(box_x), static_cast<float>(box_y), static_cast<float>(box_w), static_cast<float>(box_h)};

    DrawRectangleRec(box, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(box, 2.0f, g.ui.window_box_fgcolor);
    DrawText("Sound", box_x + padding, box_y + padding, title_font_size, g.ui.window_box_fgcolor);
    DrawText("Up/Down select, Left/Right adjust, Esc returns", box_x + padding, box_y + padding + 22, font_size, g.ui.window_box_fgcolor);

    const char* labels[] = {"Master", "Music", "Sound Effects"};
    const float values[] = {g.audio.get_master_volume(), g.audio.get_music_volume(), g.audio.get_sfx_volume()};
    int y = box_y + padding + 48;
    for (size_t i = 0; i < 3; i++) {
        const bool selected = g.ui.sound_menu_selection == i;
        DrawText(selected ? TextFormat("> %-14s %3d%%", labels[i], static_cast<int>(values[i] * 100.0f))
                          : TextFormat("  %-14s %3d%%", labels[i], static_cast<int>(values[i] * 100.0f)),
            box_x + padding,
            y,
            font_size,
            selected ? YELLOW : g.ui.window_box_fgcolor);
        y += line_height;
    }
}
