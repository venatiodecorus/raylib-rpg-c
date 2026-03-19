#include "gamestate.h"

/** @file gamestate_keybinding.cpp
 *  @brief Keybinding/profile management helpers implemented on `gamestate`.
 */


bool gamestate::is_action_pressed(const inputstate& is, gameplay_input_action_t action) const {
    const gameplay_keybinding_t& binding = keybindings[static_cast<size_t>(keyboard_profile)][static_cast<size_t>(action)];
    for (int key : binding) {
        if (key >= 0 && inputstate_is_pressed(is, key)) {
            return true;
        }
    }
    return false;
}

bool gamestate::is_action_held(const inputstate& is, gameplay_input_action_t action) const {
    const gameplay_keybinding_t& binding = keybindings[static_cast<size_t>(keyboard_profile)][static_cast<size_t>(action)];
    for (int key : binding) {
        if (key >= 0 && inputstate_is_held(is, key)) {
            return true;
        }
    }
    return false;
}

void gamestate::set_default_keybinding(keyboard_profile_t profile, gameplay_input_action_t action) {
    gameplay_keybinding_t binding = {-1, -1, -1};

    if (profile == keyboard_profile_t::FULL) {
        switch (action) {
        case gameplay_input_action_t::MOVE_UP: binding = {KEY_UP, KEY_KP_8, -1}; break;
        case gameplay_input_action_t::MOVE_DOWN: binding = {KEY_DOWN, KEY_KP_2, -1}; break;
        case gameplay_input_action_t::MOVE_LEFT: binding = {KEY_LEFT, KEY_KP_4, -1}; break;
        case gameplay_input_action_t::MOVE_RIGHT: binding = {KEY_RIGHT, KEY_KP_6, -1}; break;
        case gameplay_input_action_t::MOVE_UP_LEFT: binding = {KEY_KP_7, -1, -1}; break;
        case gameplay_input_action_t::MOVE_UP_RIGHT: binding = {KEY_KP_9, -1, -1}; break;
        case gameplay_input_action_t::MOVE_DOWN_LEFT: binding = {KEY_KP_1, -1, -1}; break;
        case gameplay_input_action_t::MOVE_DOWN_RIGHT: binding = {KEY_KP_3, -1, -1}; break;
        case gameplay_input_action_t::DIRECTION_MODE: binding = {KEY_KP_5, -1, -1}; break;
        case gameplay_input_action_t::FACE_WAIT: binding = {KEY_S, KEY_KP_5, -1}; break;
        case gameplay_input_action_t::FACE_UP: binding = {KEY_W, KEY_UP, KEY_KP_8}; break;
        case gameplay_input_action_t::FACE_DOWN: binding = {KEY_X, KEY_DOWN, KEY_KP_2}; break;
        case gameplay_input_action_t::FACE_LEFT: binding = {KEY_A, KEY_LEFT, KEY_KP_4}; break;
        case gameplay_input_action_t::FACE_RIGHT: binding = {KEY_D, KEY_RIGHT, KEY_KP_6}; break;
        case gameplay_input_action_t::FACE_UP_LEFT: binding = {KEY_Q, KEY_KP_7, -1}; break;
        case gameplay_input_action_t::FACE_UP_RIGHT: binding = {KEY_E, KEY_KP_9, -1}; break;
        case gameplay_input_action_t::FACE_DOWN_LEFT: binding = {KEY_Z, KEY_KP_1, -1}; break;
        case gameplay_input_action_t::FACE_DOWN_RIGHT: binding = {KEY_C, KEY_KP_3, -1}; break;
        case gameplay_input_action_t::FACE_ATTACK: binding = {KEY_APOSTROPHE, -1, -1}; break;
        case gameplay_input_action_t::ATTACK: binding = {KEY_A, -1, -1}; break;
        case gameplay_input_action_t::PICKUP: binding = {KEY_S, -1, -1}; break;
        case gameplay_input_action_t::STAIRS: binding = {KEY_F, -1, -1}; break;
        case gameplay_input_action_t::OPEN: binding = {KEY_D, -1, -1}; break;
        case gameplay_input_action_t::INTERACT: binding = {KEY_E, -1, -1}; break;
        case gameplay_input_action_t::INVENTORY: binding = {KEY_I, -1, -1}; break;
        case gameplay_input_action_t::PULL: binding = {KEY_SPACE, -1, -1}; break;
        case gameplay_input_action_t::HELP: binding = {KEY_SLASH, -1, -1}; break;
        case gameplay_input_action_t::OPTIONS: binding = {KEY_GRAVE, -1, -1}; break;
        case gameplay_input_action_t::CAMERA_TOGGLE: binding = {KEY_B, -1, -1}; break;
        case gameplay_input_action_t::RESTART: binding = {KEY_R, -1, -1}; break;
        case gameplay_input_action_t::TOGGLE_FULL_LIGHT: binding = {KEY_L, -1, -1}; break;
        case gameplay_input_action_t::ZOOM_IN: binding = {KEY_MINUS, -1, -1}; break;
        case gameplay_input_action_t::ZOOM_OUT: binding = {KEY_EQUAL, -1, -1}; break;
        case gameplay_input_action_t::COUNT:
        default:
            break;
        }
    } else {
        switch (action) {
        case gameplay_input_action_t::MOVE_UP: binding = {KEY_K, KEY_UP, -1}; break;
        case gameplay_input_action_t::MOVE_DOWN: binding = {KEY_J, KEY_DOWN, -1}; break;
        case gameplay_input_action_t::MOVE_LEFT: binding = {KEY_H, KEY_LEFT, -1}; break;
        case gameplay_input_action_t::MOVE_RIGHT: binding = {KEY_L, KEY_RIGHT, -1}; break;
        case gameplay_input_action_t::MOVE_UP_LEFT: binding = {KEY_Y, -1, -1}; break;
        case gameplay_input_action_t::MOVE_UP_RIGHT: binding = {KEY_U, -1, -1}; break;
        case gameplay_input_action_t::MOVE_DOWN_LEFT: binding = {KEY_B, -1, -1}; break;
        case gameplay_input_action_t::MOVE_DOWN_RIGHT: binding = {KEY_N, -1, -1}; break;
        case gameplay_input_action_t::DIRECTION_MODE: binding = {KEY_G, -1, -1}; break;
        case gameplay_input_action_t::FACE_WAIT: binding = {KEY_PERIOD, -1, -1}; break;
        case gameplay_input_action_t::FACE_UP: binding = {KEY_K, -1, -1}; break;
        case gameplay_input_action_t::FACE_DOWN: binding = {KEY_J, -1, -1}; break;
        case gameplay_input_action_t::FACE_LEFT: binding = {KEY_H, -1, -1}; break;
        case gameplay_input_action_t::FACE_RIGHT: binding = {KEY_L, -1, -1}; break;
        case gameplay_input_action_t::FACE_UP_LEFT: binding = {KEY_Y, -1, -1}; break;
        case gameplay_input_action_t::FACE_UP_RIGHT: binding = {KEY_U, -1, -1}; break;
        case gameplay_input_action_t::FACE_DOWN_LEFT: binding = {KEY_B, -1, -1}; break;
        case gameplay_input_action_t::FACE_DOWN_RIGHT: binding = {KEY_N, -1, -1}; break;
        case gameplay_input_action_t::FACE_ATTACK: binding = {KEY_A, -1, -1}; break;
        case gameplay_input_action_t::ATTACK: binding = {KEY_A, -1, -1}; break;
        case gameplay_input_action_t::PICKUP: binding = {KEY_S, -1, -1}; break;
        case gameplay_input_action_t::STAIRS: binding = {KEY_F, -1, -1}; break;
        case gameplay_input_action_t::OPEN: binding = {KEY_D, -1, -1}; break;
        case gameplay_input_action_t::INTERACT: binding = {KEY_E, -1, -1}; break;
        case gameplay_input_action_t::INVENTORY: binding = {KEY_I, -1, -1}; break;
        case gameplay_input_action_t::PULL: binding = {KEY_SPACE, -1, -1}; break;
        case gameplay_input_action_t::HELP: binding = {KEY_SLASH, -1, -1}; break;
        case gameplay_input_action_t::OPTIONS: binding = {KEY_GRAVE, -1, -1}; break;
        case gameplay_input_action_t::CAMERA_TOGGLE: binding = {KEY_V, -1, -1}; break;
        case gameplay_input_action_t::RESTART: binding = {KEY_R, -1, -1}; break;
        case gameplay_input_action_t::TOGGLE_FULL_LIGHT: binding = {KEY_SEMICOLON, -1, -1}; break;
        case gameplay_input_action_t::ZOOM_IN: binding = {KEY_MINUS, -1, -1}; break;
        case gameplay_input_action_t::ZOOM_OUT: binding = {KEY_EQUAL, -1, -1}; break;
        case gameplay_input_action_t::COUNT:
        default:
            break;
        }
    }

    keybindings[static_cast<size_t>(profile)][static_cast<size_t>(action)] = binding;
}

void gamestate::reset_default_keybindings() {
    for (int profile = 0; profile < static_cast<int>(keyboard_profile_t::COUNT); profile++) {
        reset_profile_keybindings(static_cast<keyboard_profile_t>(profile));
    }
}

void gamestate::reset_profile_keybindings(keyboard_profile_t profile) {
    for (int action = 0; action < static_cast<int>(gameplay_input_action_t::COUNT); action++) {
        set_default_keybinding(profile, static_cast<gameplay_input_action_t>(action));
    }
}

void gamestate::set_keybinding_primary(keyboard_profile_t profile, gameplay_input_action_t action, int key) {
    keybindings[static_cast<size_t>(profile)][static_cast<size_t>(action)][0] = key;
}

int gamestate::get_keybinding_primary(keyboard_profile_t profile, gameplay_input_action_t action) const {
    return keybindings[static_cast<size_t>(profile)][static_cast<size_t>(action)][0];
}

string gamestate::get_keybinding_label(keyboard_profile_t profile, gameplay_input_action_t action) const {
    const gameplay_keybinding_t& binding = keybindings[static_cast<size_t>(profile)][static_cast<size_t>(action)];
    string label;
    for (int slot = 0; slot < GAMEPLAY_KEYBINDING_SLOTS; slot++) {
        if (binding[slot] < 0) {
            continue;
        }
        if (!label.empty()) {
            label += " / ";
        }
        label += gameplay_key_name(binding[slot]);
    }
    return label.empty() ? "Unbound" : label;
}

void gamestate::open_keyboard_profile_prompt() {
    ui.display_keyboard_profile_prompt = true;
    ui.keyboard_profile_selection = static_cast<unsigned int>(keyboard_profile);
    controlmode = controlmode_t::KEYBOARD_PROFILE;
    frame_dirty = true;
}

void gamestate::apply_keyboard_profile_selection() {
    keyboard_profile = static_cast<keyboard_profile_t>(ui.keyboard_profile_selection % static_cast<unsigned int>(keyboard_profile_t::COUNT));
    keyboard_profile_confirmed = true;
    ui.display_keyboard_profile_prompt = false;
    controlmode = controlmode_t::PLAYER;
    frame_dirty = true;
}

void gamestate::handle_input_keyboard_profile_prompt(inputstate& is) {
    if (controlmode != controlmode_t::KEYBOARD_PROFILE || !ui.display_keyboard_profile_prompt) {
        return;
    }

    if (inputstate_is_pressed(is, KEY_LEFT) || inputstate_is_pressed(is, KEY_UP)) {
        ui.keyboard_profile_selection = ui.keyboard_profile_selection == 0 ? static_cast<unsigned int>(keyboard_profile_t::COUNT) - 1 : ui.keyboard_profile_selection - 1;
        frame_dirty = true;
    }
    else if (inputstate_is_pressed(is, KEY_RIGHT) || inputstate_is_pressed(is, KEY_DOWN)) {
        ui.keyboard_profile_selection = (ui.keyboard_profile_selection + 1) % static_cast<unsigned int>(keyboard_profile_t::COUNT);
        frame_dirty = true;
    }
    else if (inputstate_is_pressed(is, KEY_ENTER)) {
        audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
        apply_keyboard_profile_selection();
    }
}

void gamestate::open_controls_menu() {
    ui.display_option_menu = false;
    ui.display_controls_menu = true;
    controls_menu_waiting_for_key = false;
    ui.controls_menu_selection = 0;
    controlmode = controlmode_t::CONTROLS_MENU;
    frame_dirty = true;
}

void gamestate::close_controls_menu() {
    ui.display_controls_menu = false;
    controls_menu_waiting_for_key = false;
    controlmode = controlmode_t::PLAYER;
    frame_dirty = true;
}

void gamestate::handle_input_controls_menu(inputstate& is) {
    if (controlmode != controlmode_t::CONTROLS_MENU || !ui.display_controls_menu) {
        return;
    }

    const size_t option_count = static_cast<size_t>(gameplay_input_action_t::COUNT) + 2;
    if (controls_menu_waiting_for_key) {
        if (inputstate_is_pressed(is, KEY_ESCAPE) || inputstate_is_pressed(is, KEY_GRAVE)) {
            controls_menu_waiting_for_key = false;
            frame_dirty = true;
            return;
        }

        const int captured_key = inputstate_get_pressed_key(is);
        if (captured_key >= 0 && captured_key != KEY_ENTER) {
            set_keybinding_primary(keyboard_profile, controls_menu_pending_action, captured_key);
            controls_menu_waiting_for_key = false;
            frame_dirty = true;
        }
        return;
    }

    if (inputstate_is_pressed(is, KEY_ESCAPE) || inputstate_is_pressed(is, KEY_GRAVE)) {
        close_controls_menu();
        return;
    }
    if (inputstate_is_pressed(is, KEY_UP)) {
        ui.controls_menu_selection = ui.controls_menu_selection == 0 ? option_count - 1 : ui.controls_menu_selection - 1;
        frame_dirty = true;
        return;
    }
    if (inputstate_is_pressed(is, KEY_DOWN)) {
        ui.controls_menu_selection = (ui.controls_menu_selection + 1) % option_count;
        frame_dirty = true;
        return;
    }
    if (inputstate_is_pressed(is, KEY_LEFT) || inputstate_is_pressed(is, KEY_RIGHT) || inputstate_is_pressed(is, KEY_ENTER)) {
        if (ui.controls_menu_selection == 0) {
            keyboard_profile = keyboard_profile == keyboard_profile_t::FULL ? keyboard_profile_t::LAPTOP : keyboard_profile_t::FULL;
            frame_dirty = true;
            return;
        }
        if (ui.controls_menu_selection == 1) {
            reset_profile_keybindings(keyboard_profile);
            frame_dirty = true;
            return;
        }

        controls_menu_pending_action = static_cast<gameplay_input_action_t>(ui.controls_menu_selection - 2);
        controls_menu_waiting_for_key = true;
        frame_dirty = true;
    }
}
