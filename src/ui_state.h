/** @file ui_state.h
 *  @brief UI display flags, cursor positions, and menu selections.
 */
#pragma once

#include <raylib.h>
#include <string>
#include "entityid.h"
#include "option_menu.h"

namespace rpg {

enum class confirm_action_t {
    NONE = 0,
    QUIT,
};

constexpr Color DEFAULT_WINDOW_BOX_BGCOLOR = Color{0, 0, 255, 128};
constexpr Color DEFAULT_WINDOW_BOX_FGCOLOR = Color{255, 255, 255, 255};

struct UIState {
    // Display flags
    bool display_inventory_menu = false;
    bool display_chest_menu = false;
    bool display_action_menu = false;
    bool display_option_menu = false;
    bool display_sound_menu = false;
    bool display_window_color_menu = false;
    bool display_controls_menu = false;
    bool display_keyboard_profile_prompt = false;
    bool display_confirm_prompt = false;
    bool display_interaction_modal = false;
    bool display_level_up_modal = false;
    bool display_help_menu = false;
    bool chest_deposit_mode = false;
    bool prefer_mini_inventory_menu = false;

    // Selection / cursor state
    unsigned int inventory_menu_selection = 0;
    unsigned int level_up_selection = 0;
    unsigned int gameplay_settings_menu_selection = 0;
    unsigned int sound_menu_selection = 0;
    unsigned int window_color_menu_selection = 0;
    unsigned int keyboard_profile_selection = 0;
    unsigned int mini_inventory_visible_count = 10;
    unsigned int mini_inventory_scroll_offset = 0;
    unsigned int title_screen_selection = 0;
    unsigned int max_title_screen_selections = 2;
    size_t action_selection = 0;
    size_t controls_menu_selection = 0;
    Vector2 inventory_cursor = {0, 0};
    entityid active_chest_id = ENTITYID_INVALID;

    // Confirm prompt
    confirm_action_t confirm_action = confirm_action_t::NONE;
    std::string confirm_prompt_message;

    // Interaction modal
    entityid active_interaction_entity_id = -1;
    std::string interaction_title;
    std::string interaction_body;

    // Options menu
    option_menu options_menu;

    // Pending level ups
    int pending_level_ups = 0;

    // Window colors
    Color window_box_bgcolor = DEFAULT_WINDOW_BOX_BGCOLOR;
    Color window_box_fgcolor = DEFAULT_WINDOW_BOX_FGCOLOR;
    Color message_history_bgcolor = DEFAULT_WINDOW_BOX_BGCOLOR;
};

} // namespace rpg
