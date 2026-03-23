/** @file main_menu_scene.cpp
 *  @brief Main menu scene implementation.
 */

#include "main_menu_scene.h"
#include "gamestate.h"

void MainMenuScene::handle_input(gamestate& g, inputstate& is) {
    if (inputstate_is_pressed(is, KEY_ENTER) || inputstate_is_pressed(is, KEY_SPACE)) {
        if (g.ui.title_screen_selection == 0) {
            g.current_scene = scene_t::CHARACTER_CREATION;
            g.frame_dirty = true;
        }
        g.audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
    }
    else if (inputstate_is_pressed(is, KEY_DOWN)) {
        g.ui.title_screen_selection++;
        if (g.ui.title_screen_selection >= g.ui.max_title_screen_selections) {
            g.ui.title_screen_selection = 0;
        }
        g.audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
    }
    else if (inputstate_is_pressed(is, KEY_UP)) {
        g.ui.title_screen_selection--;
        if (g.ui.title_screen_selection < 0) {
            g.ui.title_screen_selection = g.ui.max_title_screen_selections - 1;
        }
        g.audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
    }
    else if (inputstate_is_pressed(is, KEY_ESCAPE)) {
        g.do_quit = true;
        g.audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
    }
    g.frame_dirty = true;
}
