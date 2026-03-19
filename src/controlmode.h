/** @file controlmode.h
 *  @brief Input-routing modes for gameplay and UI substates.
 */

#pragma once

/// @brief Active control context used to dispatch input within a scene.
enum class controlmode_t {
    CAMERA = 0,
    PLAYER,
    INVENTORY,
    CHEST,
    ACTION_MENU,
    OPTION_MENU,
    SOUND_MENU,
    WINDOW_COLOR_MENU,
    CONTROLS_MENU,
    HELP_MENU,
    INTERACTION,
    LEVEL_UP,
    KEYBOARD_PROFILE,
    CONFIRM_PROMPT,
    COUNT
};
