/** @file gameplay_scene.cpp
 *  @brief Gameplay scene implementation.
 */

#include "gameplay_scene.h"
#include "gamestate.h"

void GameplayScene::on_enter(gamestate& g) {
    minfo("GameplayScene::on_enter");
    g.frame_dirty = true;
}

void GameplayScene::on_exit(gamestate& g) {
    minfo("GameplayScene::on_exit");
}

void GameplayScene::handle_input(gamestate& g, inputstate& is) {
    g.handle_input_gameplay_scene(is);
}

void GameplayScene::update(gamestate& g, inputstate& is) {
    if (!g.update_player_tiles_explored()) {
        merror3("update player tiles explored failed");
    }
    if (!g.update_player_state()) {
        merror3("update player state failed");
    }
    g.update_npcs_state();
    g.handle_npcs();
    g.damage_popups_sys.update(g.test ? (1.0f / DEFAULT_TARGET_FPS) : std::max(GetFrameTime(), 1.0f / 240.0f));
    if (g.damage_popups_sys.dirty) {
        g.frame_dirty = true;
        g.damage_popups_sys.dirty = false;
    }
#ifdef DEBUG
    g.update_debug_panel_buffer(is);
#endif
    g.currenttime = time(NULL);
    g.currenttimetm = localtime(&g.currenttime);
    strftime(g.currenttimebuf, GAMESTATE_SIZEOFTIMEBUF, "Current Time: %Y-%m-%d %H:%M:%S", g.currenttimetm);
    g.ticks++;
}
