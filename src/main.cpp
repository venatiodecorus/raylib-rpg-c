/** @file main.cpp
 *  @brief Application entrypoint, process-lifetime globals, and outer game loop control.
 */

#include "gamestate.h"
#include "inputstate.h"
#include "libdraw.h"
#include "libgame_defines.h"
#ifdef WEB
#include <emscripten/emscripten.h>
#endif

struct AppContext {
    gamestate g;
    inputstate is;
    rpg::Renderer renderer;
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
    ctx->g.tick(ctx->is);
    drawframe(ctx->g, ctx->renderer);
    ctx->g.advance_animation_phase();
    ctx->g.audio.flush(ctx->g.test);
    ctx->g.audio.update_music(ctx->g.random.mt, ctx->g.test);
    ctx->g.finalize_render_feedback();
    if (ctx->g.do_restart) {
        msuccess("Restarting game...");
        ctx->g.audio.shutdown();
        ctx->g.restart_game();
        ctx->g.audio.init(ctx->g.random.mt, ctx->g.test);
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
    ctx->g.logic_init();
    libdraw_init(ctx->g, ctx->renderer);
    ctx->g.audio.init(ctx->g.random.mt, ctx->g.test);
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
