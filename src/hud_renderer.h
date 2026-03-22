/** @file hud_renderer.h
 *  @brief Player HUD, message history, message box, interaction modal, look panel, level-up modal rendering.
 */

#pragma once

#include "gamestate.h"

namespace rpg
{

class HudRenderer {
public:
    void draw_hud(gamestate& g);
    void draw_look_panel(gamestate& g);
    void draw_message_history(gamestate& g);
    void draw_message_box(gamestate& g);
    void draw_interaction_modal(gamestate& g);
    void draw_level_up_modal(gamestate& g);
};

} // namespace rpg
