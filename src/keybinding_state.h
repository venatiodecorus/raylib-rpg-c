#pragma once

#include "gameplay_keybindings.h"
#include <array>

namespace rpg {

struct KeybindingState {
    keyboard_profile_t keyboard_profile = keyboard_profile_t::FULL;
    bool keyboard_profile_confirmed = false;
    bool controls_menu_waiting_for_key = false;
    gameplay_input_action_t controls_menu_pending_action = gameplay_input_action_t::MOVE_UP;
    std::array<std::array<gameplay_keybinding_t, static_cast<size_t>(gameplay_input_action_t::COUNT)>, static_cast<size_t>(keyboard_profile_t::COUNT)> keybindings{};
};

} // namespace rpg
