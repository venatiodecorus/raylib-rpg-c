#pragma once

#include "debugpanel.h"
#include <raylib.h>

struct PresentationState {
    debugpanel_t debugpanel{};
    bool debugpanelon = false;
    bool gridon = false;
    bool cam_lockon = false;
    bool cam_changed = false;
    unsigned int fadealpha = 0;
    unsigned int targetwidth = 0;
    unsigned int targetheight = 0;
    unsigned int windowwidth = 0;
    unsigned int windowheight = 0;
    unsigned int font_size = 0;
    int pad = 0;
    float line_spacing = 1.0f;
    Vector2 last_click_screen_pos = Vector2{-1, -1};
    Camera2D cam2d{};
    Font font{};
};
