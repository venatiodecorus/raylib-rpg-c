/** @file title_scene.cpp
 *  @brief Title screen scene implementation.
 */

#include "title_scene.h"
#include "gamestate.h"

void TitleScene::handle_input(gamestate& g, inputstate& is) {
    if (inputstate_is_pressed(is, KEY_ESCAPE)) {
        g.do_quit = true;
        return;
    }
    if (inputstate_is_pressed(is, KEY_ENTER) || inputstate_is_pressed(is, KEY_SPACE)) {
        g.current_scene = scene_t::MAIN_MENU;
        g.frame_dirty = true;
        g.audio.queue("sfx/Minifantasy_Dungeon_SFX/02_chest_close_1.wav");
    }
}
