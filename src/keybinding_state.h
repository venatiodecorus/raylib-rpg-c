#pragma once

#include "gameplay_keybindings.h"
#include <array>

namespace rpg {

struct KeybindingState {
    keyboard_profile_t keyboard_profile = KEYBOARD_PROFILE_FULL;
    bool keyboard_profile_confirmed = false;
    bool controls_menu_waiting_for_key = false;
    gameplay_input_action_t controls_menu_pending_action = INPUT_ACTION_MOVE_UP;
    std::array<std::array<gameplay_keybinding_t, INPUT_ACTION_COUNT>, KEYBOARD_PROFILE_COUNT> keybindings{};
};

} // namespace rpg
