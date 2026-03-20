#include "draw_help_menu.h"

void draw_help_menu(gamestate& g) {
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
        "@evildojo666",
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
    DrawRectangleLinesEx({static_cast<float>(box_x), static_cast<float>(box_y), static_cast<float>(box_width), static_cast<float>(box_height)}, 2, g.ui.window_box_fgcolor);

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
