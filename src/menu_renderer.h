/** @file menu_renderer.h
 *  @brief In-game overlay menu rendering: action, options, help, controls, sound, window color.
 */

#pragma once

#include "gamestate.h"

namespace rpg
{

class MenuRenderer {
public:
    void draw_action_menu(gamestate& g);
    void draw_option_menu(gamestate& g);
    void draw_help_menu(gamestate& g);
    void draw_controls_menu(gamestate& g);
    void draw_sound_menu(gamestate& g);
    void draw_window_color_menu(gamestate& g);
};

} // namespace rpg
