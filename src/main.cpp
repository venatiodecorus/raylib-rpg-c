/** @file main.cpp
 *  @brief Application entrypoint, process-lifetime globals, and outer game loop control.
 */

#include "character_creation_scene.h"
#include "gameplay_scene.h"
#include "gamestate.h"
#include "inputstate.h"
#include "libdraw.h"
#include "libgame_defines.h"
#include "main_menu_scene.h"
#include "title_scene.h"
#ifdef WEB
#include <emscripten/emscripten.h>
#endif

struct AppContext {
    gamestate g;
    inputstate is;
    rpg::Renderer renderer;
    TitleScene title_scene;
    MainMenuScene main_menu_scene;
    CharacterCreationScene character_creation_scene;
    GameplayScene gameplay_scene;
    Scene* scene;

    Scene* scene_for(scene_t s) {
        switch (s) {
        case scene_t::TITLE: return &title_scene;
        case scene_t::MAIN_MENU: return &main_menu_scene;
        case scene_t::CHARACTER_CREATION: return &character_creation_scene;
        case scene_t::GAMEPLAY: return &gameplay_scene;
        default: return &title_scene;
        }
    }

    void apply_scene_change() {
        Scene* next = scene_for(g.current_scene);
        if (next != scene) {
            if (scene) {
                scene->on_exit(g);
            }
            scene = next;
            if (scene) {
                scene->on_enter(g);
            }
            g.frame_dirty = true;
        }
    }
};

/**
 * @brief Advance one outer frame of the application loop.
 *
 * Pulls fresh input, advances gameplay simulation, renders the current frame,
 * and performs an in-process gameplay restart when requested.
 */
static void gameloop(void* arg) {
    AppContext* ctx = static_cast<AppContext*>(arg);
    inputstate_update(ctx->is);
    if (ctx->scene) {
        const scene_t before = ctx->g.current_scene;
        ctx->scene->handle_input(ctx->g, ctx->is);
        if (ctx->g.current_scene != before) {
            ctx->apply_scene_change();
        }
        ctx->scene->update(ctx->g, ctx->is);
    }
    else {
        ctx->g.tick(ctx->is);
    }
    drawframe(ctx->g, ctx->renderer);
    ctx->g.advance_animation_phase();
    ctx->g.audio.flush(ctx->g.test);
    ctx->g.audio.update_music(ctx->g.random.mt, ctx->g.test);
    ctx->g.finalize_render_feedback();
    if (ctx->g.do_restart) {
        msuccess("Restarting game...");
        if (ctx->scene) {
            ctx->scene->on_exit(ctx->g);
        }
        ctx->g.audio.shutdown();
        ctx->g.restart_game();
        ctx->g.audio.init(ctx->g.random.mt, ctx->g.test);
        ctx->scene = &ctx->title_scene;
        ctx->g.current_scene = scene_t::TITLE;
    }
}

/**
 * @brief Initialize the runtime and enter the platform-specific outer loop.
 *
 * Native builds run an explicit loop until gameplay requests shutdown. Web
 * builds hand control to Emscripten's browser-managed loop instead.
 *
 * @return Process exit code.
 */
int main() {
    AppContext* ctx = new AppContext();
    ctx->scene = nullptr;
    ctx->g.logic_init();
    libdraw_init(ctx->g, ctx->renderer);
    ctx->g.audio.init(ctx->g.random.mt, ctx->g.test);

    ctx->scene = &ctx->title_scene;
    ctx->g.current_scene = scene_t::TITLE;
#ifndef WEB
    while (!libdraw_windowshouldclose(ctx->g)) {
        gameloop(ctx);
    }
#else
    emscripten_set_main_loop_arg(gameloop, ctx, 0, 1);
#endif
    ctx->g.audio.shutdown();
    libdraw_close(ctx->renderer);
    ctx->g.logic_close();
    delete ctx;
    return 0;
}
