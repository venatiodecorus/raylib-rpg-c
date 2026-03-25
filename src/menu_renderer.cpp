/** @file menu_renderer.cpp
 *  @brief MenuRenderer implementation — action, options, help, controls, sound, window color menus.
 */

#include "menu_renderer.h"

#include "gameplay_keybindings.h"

namespace rpg
{

void MenuRenderer::draw_action_menu(gamestate& g) {
    constexpr int fontsize = 10;
    constexpr int hp = 20;
    constexpr float x = 10;
    constexpr float y = 10;
    constexpr float rotation = 0;
    constexpr int thickness = 1;
    constexpr float xp = 10;
    constexpr float yp = 10;
    constexpr float wp = 20;
    constexpr Vector2 origin = {0, 0};
    const vector<string> actions = {"Attack", "Open", "Inventory"};
    const float h = fontsize * actions.size();
    const string s = "> Inventory";
    const float w = MeasureText(s.c_str(), fontsize);
    const Rectangle r = {x, y, w + wp, h + hp};
    DrawRectanglePro(r, origin, rotation, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(r, thickness, g.ui.window_box_fgcolor);
    for (size_t i = 0; i < actions.size(); i++) {
        if (g.ui.action_selection == i) {
            DrawText(TextFormat("> %s", actions[i].c_str()), x + xp, y + yp + (fontsize * i), fontsize, YELLOW);
        }
        else {
            DrawText(TextFormat("  %s", actions[i].c_str()), x + xp, y + yp + (fontsize * i), fontsize, g.ui.window_box_fgcolor);
        }
    }
}

void MenuRenderer::draw_option_menu(gamestate& g) {
    constexpr float x = 10, y = 10, p = 20, pa = 10, rotation = 0;
    constexpr int fsize = 10, thickness = 1;
    constexpr Vector2 origin = {0, 0};
    float max_w = 0;
    for (size_t i = 0; i < g.ui.options_menu.get_option_count(); i++) {
        const option_type option = g.ui.options_menu.get_option(i);
        const string option_label = option == option_type::INVENTORY_MENU ? TextFormat("inventory menu: %s", g.ui.prefer_mini_inventory_menu ? "mini" : "full")
                                                                          : g.ui.options_menu.get_option_str(option);
        const char* spaced_str = TextFormat("  %s", option_label.c_str());
        const float w = MeasureText(spaced_str, fsize);
        if (w > max_w) {
            max_w = w;
        }
    }
    const float h = fsize * g.ui.options_menu.get_option_count();
    const float padded_w = max_w + p, padded_h = h + p;
    const Rectangle r = {x, y, padded_w, padded_h};
    DrawRectanglePro(r, origin, rotation, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(r, thickness, g.ui.window_box_fgcolor);
    for (size_t i = 0; i < g.ui.options_menu.get_option_count(); i++) {
        constexpr int x0 = x + pa;
        const int y0 = y + pa + fsize * i;
        const option_type otype = g.ui.options_menu.get_option(i);
        const string ostr = otype == option_type::INVENTORY_MENU ? TextFormat("inventory menu: %s", g.ui.prefer_mini_inventory_menu ? "mini" : "full")
                                                                 : g.ui.options_menu.get_option_str(otype);
        const char* cstr = ostr.c_str();
        if (g.ui.options_menu.get_selection() == i) {
            DrawText(TextFormat("> %s", cstr), x0, y0, fsize, YELLOW);
        }
        else {
            DrawText(TextFormat("  %s", cstr), x0, y0, fsize, g.ui.window_box_fgcolor);
        }
    }
}

void MenuRenderer::draw_help_menu(gamestate& g) {
    const string text = TextFormat(
        "Help Menu\n"
        "\n"
        "Profile: %s\n"
        "Move Up: %s\n"
        "Move Down: %s\n"
        "Move Left / Right: %s / %s\n"
        "Move Diagonals: %s %s %s %s\n"
        "Face Mode: %s\n"
        "Face Attack: %s\n"
        "Camera mode: %s\n"
        "Zoom: %s and %s\n"
        "\n"
        "Pick up item: %s\n"
        "Attack: %s\n"
        "Pull: %s\n"
        "Open door / chest: %s\n"
        "Interact / examine: %s\n"
        "Use stairs: %s\n"
        "Toggle full light: %s\n"
        "Level-up select: arrows + enter\n"
        "\n"
        "Inventory: %s\n"
        "Inventory equip/use: e or enter\n"
        "Inventory drop: q\n"
        "Inventory close: esc or inventory key\n"
        "\n"
        "Options: %s\n"
        "Open this help menu: %s\n"
        "Debug panel: p\n"
        "Quit prompt: esc\n"
        "\n"
        "@venatiodecorus",
        keyboard_profile_label(g.keyboard_profile),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::MOVE_UP).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::MOVE_DOWN).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::MOVE_LEFT).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::MOVE_RIGHT).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::MOVE_UP_LEFT).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::MOVE_UP_RIGHT).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::MOVE_DOWN_LEFT).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::MOVE_DOWN_RIGHT).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::DIRECTION_MODE).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::FACE_ATTACK).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::CAMERA_TOGGLE).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::ZOOM_IN).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::ZOOM_OUT).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::PICKUP).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::ATTACK).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::PULL).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::OPEN).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::INTERACT).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::STAIRS).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::TOGGLE_FULL_LIGHT).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::INVENTORY).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::OPTIONS).c_str(),
        g.get_keybinding_label(g.keyboard_profile, gameplay_input_action_t::HELP).c_str());

    constexpr int font_size = 20;
    constexpr int line_spacing = 4;
    const int padding = DEFAULT_PAD * 2;

    int max_text_width = 0;
    int line_count = 0;
    size_t line_start = 0;
    while (line_start <= text.size()) {
        const size_t line_end = text.find('\n', line_start);
        const string line = line_end == string::npos ? text.substr(line_start) : text.substr(line_start, line_end - line_start);
        const int line_width = MeasureText(line.c_str(), font_size);
        if (line_width > max_text_width) {
            max_text_width = line_width;
        }
        line_count++;
        if (line_end == string::npos) {
            break;
        }
        line_start = line_end + 1;
    }

    const int text_height = line_count * font_size + (line_count - 1) * line_spacing;
    const int box_width = max_text_width + padding * 2;
    const int box_height = text_height + padding * 2;
    const int box_x = (DEFAULT_TARGET_WIDTH - box_width) / 2;
    const int box_y = (DEFAULT_TARGET_HEIGHT - box_height) / 2;

    DrawRectangle(box_x, box_y, box_width, box_height, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(
        {static_cast<float>(box_x), static_cast<float>(box_y), static_cast<float>(box_width), static_cast<float>(box_height)}, 2, g.ui.window_box_fgcolor);

    const int text_x = box_x + padding;
    int text_y = box_y + padding;
    line_start = 0;
    while (line_start <= text.size()) {
        const size_t line_end = text.find('\n', line_start);
        const string line = line_end == string::npos ? text.substr(line_start) : text.substr(line_start, line_end - line_start);
        DrawText(line.c_str(), text_x, text_y, font_size, g.ui.window_box_fgcolor);
        text_y += font_size + line_spacing;
        if (line_end == string::npos) {
            break;
        }
        line_start = line_end + 1;
    }
}

void MenuRenderer::draw_controls_menu(gamestate& g) {
    if (!g.ui.display_controls_menu) {
        return;
    }

    constexpr int font_size = 10;
    constexpr int line_height = 12;
    constexpr int padding = 12;
    const int visible_rows = static_cast<int>(gameplay_input_action_t::COUNT) + 4;
    const int box_w = 500;
    const int box_h = visible_rows * line_height + padding * 2 + 24;
    const int box_x = (DEFAULT_TARGET_WIDTH - box_w) / 2;
    const int box_y = (DEFAULT_TARGET_HEIGHT - box_h) / 2;
    const Rectangle box = {static_cast<float>(box_x), static_cast<float>(box_y), static_cast<float>(box_w), static_cast<float>(box_h)};

    DrawRectangleRec(box, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(box, 2.0f, g.ui.window_box_fgcolor);
    DrawText("Keyboard Controls", box_x + padding, box_y + padding, 18, g.ui.window_box_fgcolor);
    const char* subtitle = g.controls_menu_waiting_for_key
                               ? TextFormat("Press a new key for %s. Esc cancels.", gameplay_input_action_label(g.controls_menu_pending_action))
                               : "Up/Down select, Enter edit, Left/Right swap profile, Esc closes";
    DrawText(subtitle, box_x + padding, box_y + padding + 20, font_size, g.ui.window_box_fgcolor);

    int y = box_y + padding + 38;
    const bool profile_selected = g.ui.controls_menu_selection == 0;
    DrawText(
        profile_selected ? TextFormat("> Profile: %s", keyboard_profile_label(g.keyboard_profile))
                         : TextFormat("  Profile: %s", keyboard_profile_label(g.keyboard_profile)),
        box_x + padding,
        y,
        font_size,
        profile_selected ? YELLOW : g.ui.window_box_fgcolor);
    y += line_height;

    const bool reset_selected = g.ui.controls_menu_selection == 1;
    DrawText(
        reset_selected ? "> Reset Current Profile To Defaults" : "  Reset Current Profile To Defaults",
        box_x + padding,
        y,
        font_size,
        reset_selected ? YELLOW : g.ui.window_box_fgcolor);
    y += line_height;

    for (int action = 0; action < static_cast<int>(gameplay_input_action_t::COUNT); action++) {
        const bool selected = g.ui.controls_menu_selection == static_cast<size_t>(action + 2);
        const char* action_label = gameplay_input_action_label(static_cast<gameplay_input_action_t>(action));
        const string binding = g.get_keybinding_label(g.keyboard_profile, static_cast<gameplay_input_action_t>(action));
        DrawText(
            selected ? TextFormat("> %-22s %s", action_label, binding.c_str()) : TextFormat("  %-22s %s", action_label, binding.c_str()),
            box_x + padding,
            y,
            font_size,
            selected ? YELLOW : g.ui.window_box_fgcolor);
        y += line_height;
    }
}

void MenuRenderer::draw_sound_menu(gamestate& g) {
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
        DrawText(
            selected ? TextFormat("> %-14s %3d%%", labels[i], static_cast<int>(values[i] * 100.0f))
                     : TextFormat("  %-14s %3d%%", labels[i], static_cast<int>(values[i] * 100.0f)),
            box_x + padding,
            y,
            font_size,
            selected ? YELLOW : g.ui.window_box_fgcolor);
        y += line_height;
    }
}

void MenuRenderer::draw_window_color_menu(gamestate& g) {
    if (!g.ui.display_window_color_menu) {
        return;
    }

    constexpr int font_size = 11;
    constexpr int title_font_size = 18;
    constexpr int line_height = 14;
    constexpr int padding = 12;
    const int box_w = 420;
    const int box_h = 220;
    const int box_x = (DEFAULT_TARGET_WIDTH - box_w) / 2;
    const int box_y = (DEFAULT_TARGET_HEIGHT - box_h) / 2;
    const Rectangle box = {static_cast<float>(box_x), static_cast<float>(box_y), static_cast<float>(box_w), static_cast<float>(box_h)};

    DrawRectangleRec(box, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(box, 2.0f, g.ui.window_box_fgcolor);
    DrawText("Window Box Colors", box_x + padding, box_y + padding, title_font_size, g.ui.window_box_fgcolor);
    DrawText("Up/Down select, Left/Right adjust, Enter reset, Esc returns", box_x + padding, box_y + padding + 22, font_size, g.ui.window_box_fgcolor);

    const unsigned char values[] = {
        g.ui.window_box_bgcolor.r,
        g.ui.window_box_bgcolor.g,
        g.ui.window_box_bgcolor.b,
        g.ui.window_box_bgcolor.a,
        g.ui.window_box_fgcolor.r,
        g.ui.window_box_fgcolor.g,
        g.ui.window_box_fgcolor.b,
        g.ui.window_box_fgcolor.a};
    const char* labels[] = {
        "Background R", "Background G", "Background B", "Background A", "Foreground R", "Foreground G", "Foreground B", "Foreground A", "Reset Defaults"};

    int y = box_y + padding + 48;
    for (size_t i = 0; i < 9; i++) {
        const bool selected = g.ui.window_color_menu_selection == i;
        const char* text =
            i < 8 ? TextFormat("%c %-14s %3d", selected ? '>' : ' ', labels[i], values[i]) : (selected ? "> Reset Defaults" : "  Reset Defaults");
        DrawText(text, box_x + padding, y, font_size, selected ? YELLOW : g.ui.window_box_fgcolor);
        y += line_height;
    }

    Rectangle preview = {static_cast<float>(box_x + box_w - 112), static_cast<float>(box_y + 60), 84.0f, 84.0f};
    DrawRectangleRec(preview, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(preview, 2.0f, g.ui.window_box_fgcolor);
    DrawText("Preview", static_cast<int>(preview.x) + 10, static_cast<int>(preview.y) + 32, font_size, g.ui.window_box_fgcolor);
}

} // namespace rpg
