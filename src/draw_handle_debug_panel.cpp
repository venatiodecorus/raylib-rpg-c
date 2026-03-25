#include "draw_handle_debug_panel.h"

#include <cstdio>
#include <cstring>
#include <raylib.h>

void draw_debug_panel(gamestate& g) {
    minfo3("draw debug panel");
    constexpr int fontsize = 20;
    constexpr float yp = 10;
    constexpr float wp = 20;
    constexpr float hp = 20;
    constexpr float xp = 5;
    constexpr float rotation = 0;
    constexpr int thickness = 1;
    constexpr float h = fontsize * 30;
    constexpr Vector2 origin = {0, 0};

    const float w = MeasureText(g.debugpanel.buffer, fontsize);
    const float x = g.targetwidth - (w + wp);
    const float y = g.targetheight - (h + hp);
    const Rectangle r = {x, y - yp, w + wp, h + hp};

    DrawRectanglePro(r, origin, rotation, g.get_debug_panel_bgcolor());
    DrawRectangleLinesEx(r, thickness, g.ui.window_box_fgcolor);
    DrawText(g.debugpanel.buffer, x + xp, y, fontsize, g.ui.window_box_fgcolor);
    msuccess3("draw debug panel");
}

void update_debug_panel(gamestate& g) {
    minfo3("update debug panel");
    if (g.hero_id == INVALID) {
        return;
    }

    char tmp[1024] = {0};
    snprintf(tmp, sizeof(tmp), "@venatiodecorus");
    strncat(g.debugpanel.buffer, tmp, sizeof(g.debugpanel.buffer) - strlen(g.debugpanel.buffer) - 1);
    msuccess3("update debug panel");
}

void handle_debug_panel(gamestate& g) {
    minfo3("handle debug panel");
    if (g.debugpanelon) {
        update_debug_panel(g);
        draw_debug_panel(g);
    }
    msuccess3("handle debug panel");
}
