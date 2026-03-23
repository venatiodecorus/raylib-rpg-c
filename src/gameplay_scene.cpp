/** @file gameplay_scene.cpp
 *  @brief Gameplay scene implementation.
 */

#include "gameplay_scene.h"
#include "gamestate.h"

void GameplayScene::on_enter(gamestate& g) {
    minfo("GameplayScene::on_enter");

    // Create the dungeon, NPCs, and items if this is a fresh game.
    if (!g.d.is_initialized) {
        g.logic_init();
    }

    // Create the player from character-creation parameters if not yet spawned.
    if (g.hero_id == ENTITYID_INVALID) {
        int myhd = g.chara_creation.hitdie;
        int maxhp_roll = -1;
        while (maxhp_roll < 1) {
            maxhp_roll = do_roll_best_of_3(vec3{1, myhd, 0}) + get_stat_bonus(g.chara_creation.constitution);
        }
        const vec3 start_loc = vec3{2, 2, 0};
        const string player_name = g.chara_creation.name.empty() ? "hero" : g.chara_creation.name;
        g.create_player_at_with(start_loc, player_name, g.player_init(maxhp_roll));
        massert(g.hero_id != ENTITYID_INVALID, "hero_id is invalid after player creation");
        g.make_all_npcs_target_player();

        if (!g.keyboard_profile_confirmed) {
            g.open_keyboard_profile_prompt();
        }
    }

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
