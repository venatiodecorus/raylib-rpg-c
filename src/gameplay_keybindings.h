/** @file gameplay_keybindings.h
 *  @brief Keyboard profiles, gameplay input actions, and key-label helpers.
 */

#pragma once

#include <array>
#include <cctype>
#include <raylib.h>
#include <string>

using std::array;
using std::string;

/// @brief Built-in keyboard layouts offered by the controls UI.
enum class keyboard_profile_t {
    FULL = 0,
    LAPTOP,
    COUNT
};

/// @brief Bindable gameplay actions exposed through the controls system.
enum class gameplay_input_action_t {
    MOVE_UP = 0,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP_LEFT,
    MOVE_UP_RIGHT,
    MOVE_DOWN_LEFT,
    MOVE_DOWN_RIGHT,
    DIRECTION_MODE,
    FACE_WAIT,
    FACE_UP,
    FACE_DOWN,
    FACE_LEFT,
    FACE_RIGHT,
    FACE_UP_LEFT,
    FACE_UP_RIGHT,
    FACE_DOWN_LEFT,
    FACE_DOWN_RIGHT,
    FACE_ATTACK,
    ATTACK,
    PICKUP,
    STAIRS,
    OPEN,
    INTERACT,
    INVENTORY,
    PULL,
    HELP,
    OPTIONS,
    CAMERA_TOGGLE,
    RESTART,
    TOGGLE_FULL_LIGHT,
    ZOOM_IN,
    ZOOM_OUT,
    COUNT
};

/// @brief Number of key slots stored for each bindable gameplay action.
static constexpr int GAMEPLAY_KEYBINDING_SLOTS = 3;
/// @brief Fixed-size binding record storing primary and alternate keys for one action.
using gameplay_keybinding_t = array<int, GAMEPLAY_KEYBINDING_SLOTS>;

/** @brief Return the user-facing label for a keyboard profile. */
static inline const char* keyboard_profile_label(keyboard_profile_t profile) {
    switch (profile) {
    case keyboard_profile_t::FULL:
        return "Full Keyboard";
    case keyboard_profile_t::LAPTOP:
        return "Laptop Keyboard";
    case keyboard_profile_t::COUNT:
    default:
        break;
    }
    return "Unknown";
}

/** @brief Return the user-facing label for a bindable gameplay action. */
static inline const char* gameplay_input_action_label(gameplay_input_action_t action) {
    switch (action) {
    case gameplay_input_action_t::MOVE_UP: return "Move Up";
    case gameplay_input_action_t::MOVE_DOWN: return "Move Down";
    case gameplay_input_action_t::MOVE_LEFT: return "Move Left";
    case gameplay_input_action_t::MOVE_RIGHT: return "Move Right";
    case gameplay_input_action_t::MOVE_UP_LEFT: return "Move Up-Left";
    case gameplay_input_action_t::MOVE_UP_RIGHT: return "Move Up-Right";
    case gameplay_input_action_t::MOVE_DOWN_LEFT: return "Move Down-Left";
    case gameplay_input_action_t::MOVE_DOWN_RIGHT: return "Move Down-Right";
    case gameplay_input_action_t::DIRECTION_MODE: return "Face Direction Mode";
    case gameplay_input_action_t::FACE_WAIT: return "Face Mode Wait";
    case gameplay_input_action_t::FACE_UP: return "Face Up";
    case gameplay_input_action_t::FACE_DOWN: return "Face Down";
    case gameplay_input_action_t::FACE_LEFT: return "Face Left";
    case gameplay_input_action_t::FACE_RIGHT: return "Face Right";
    case gameplay_input_action_t::FACE_UP_LEFT: return "Face Up-Left";
    case gameplay_input_action_t::FACE_UP_RIGHT: return "Face Up-Right";
    case gameplay_input_action_t::FACE_DOWN_LEFT: return "Face Down-Left";
    case gameplay_input_action_t::FACE_DOWN_RIGHT: return "Face Down-Right";
    case gameplay_input_action_t::FACE_ATTACK: return "Face Mode Attack";
    case gameplay_input_action_t::ATTACK: return "Attack";
    case gameplay_input_action_t::PICKUP: return "Pick Up Item";
    case gameplay_input_action_t::STAIRS: return "Use Stairs";
    case gameplay_input_action_t::OPEN: return "Open Door / Chest";
    case gameplay_input_action_t::INTERACT: return "Interact / Examine";
    case gameplay_input_action_t::INVENTORY: return "Inventory";
    case gameplay_input_action_t::PULL: return "Pull";
    case gameplay_input_action_t::HELP: return "Help";
    case gameplay_input_action_t::OPTIONS: return "Options";
    case gameplay_input_action_t::CAMERA_TOGGLE: return "Camera Mode";
    case gameplay_input_action_t::RESTART: return "Restart";
    case gameplay_input_action_t::TOGGLE_FULL_LIGHT: return "Toggle Full Light";
    case gameplay_input_action_t::ZOOM_IN: return "Zoom In";
    case gameplay_input_action_t::ZOOM_OUT: return "Zoom Out";
    case gameplay_input_action_t::COUNT:
    default:
        break;
    }
    return "Unknown Action";
}

/** @brief Convert a Raylib key code into a compact UI display string. */
static inline string gameplay_key_name(int key) {
    switch (key) {
    case -1: return "Unbound";
    case KEY_UP: return "Up";
    case KEY_DOWN: return "Down";
    case KEY_LEFT: return "Left";
    case KEY_RIGHT: return "Right";
    case KEY_ENTER: return "Enter";
    case KEY_ESCAPE: return "Esc";
    case KEY_SPACE: return "Space";
    case KEY_TAB: return "Tab";
    case KEY_BACKSPACE: return "Backspace";
    case KEY_SLASH: return "/";
    case KEY_PERIOD: return ".";
    case KEY_COMMA: return ",";
    case KEY_MINUS: return "-";
    case KEY_EQUAL: return "=";
    case KEY_GRAVE: return "`";
    case KEY_APOSTROPHE: return "'";
    case KEY_SEMICOLON: return ";";
    case KEY_LEFT_SHIFT: return "Left Shift";
    case KEY_RIGHT_SHIFT: return "Right Shift";
    case KEY_KP_1: return "KP1";
    case KEY_KP_2: return "KP2";
    case KEY_KP_3: return "KP3";
    case KEY_KP_4: return "KP4";
    case KEY_KP_5: return "KP5";
    case KEY_KP_6: return "KP6";
    case KEY_KP_7: return "KP7";
    case KEY_KP_8: return "KP8";
    case KEY_KP_9: return "KP9";
    default:
        break;
    }

    if (key >= KEY_A && key <= KEY_Z) {
        const char c = static_cast<char>('A' + (key - KEY_A));
        return string(1, c);
    }
    if (key >= KEY_ZERO && key <= KEY_NINE) {
        const char c = static_cast<char>('0' + (key - KEY_ZERO));
        return string(1, c);
    }
    if (key >= 32 && key <= 126) {
        const char c = static_cast<char>(std::toupper(key));
        return string(1, c);
    }
    return TextFormat("Key %d", key);
}
