/** @file gamestate_options_impl.h
 *  @brief Audio and window-color options helpers implemented on `gamestate`.
 */

#pragma once

#include "libdraw_context.h"
#include <algorithm>

inline void gamestate::open_sound_menu() {
    display_option_menu = false;
    display_sound_menu = true;
    sound_menu_selection = 0;
    controlmode = CONTROLMODE_SOUND_MENU;
    frame_dirty = true;
}

inline void gamestate::close_sound_menu() {
    display_sound_menu = false;
    display_option_menu = true;
    controlmode = CONTROLMODE_OPTION_MENU;
    frame_dirty = true;
}

inline void gamestate::open_window_color_menu() {
    display_option_menu = false;
    display_window_color_menu = true;
    window_color_menu_selection = 0;
    controlmode = CONTROLMODE_WINDOW_COLOR_MENU;
    frame_dirty = true;
}

inline void gamestate::close_window_color_menu() {
    display_window_color_menu = false;
    display_option_menu = true;
    controlmode = CONTROLMODE_OPTION_MENU;
    frame_dirty = true;
}

inline void gamestate::adjust_window_box_bg_channel(size_t channel, int dir) {
    unsigned char* values[] = {&window_box_bgcolor.r, &window_box_bgcolor.g, &window_box_bgcolor.b, &window_box_bgcolor.a};
    if (channel >= 4) {
        return;
    }
    const int adjusted = std::clamp(static_cast<int>(*values[channel]) + dir, 0, 255);
    *values[channel] = static_cast<unsigned char>(adjusted);
    message_history_bgcolor = window_box_bgcolor;
    frame_dirty = true;
}

inline void gamestate::adjust_window_box_fg_channel(size_t channel, int dir) {
    unsigned char* values[] = {&window_box_fgcolor.r, &window_box_fgcolor.g, &window_box_fgcolor.b, &window_box_fgcolor.a};
    if (channel >= 4) {
        return;
    }
    const int adjusted = std::clamp(static_cast<int>(*values[channel]) + dir, 0, 255);
    *values[channel] = static_cast<unsigned char>(adjusted);
    frame_dirty = true;
}

inline void gamestate::reset_window_box_colors() {
    window_box_bgcolor = DEFAULT_WINDOW_BOX_BGCOLOR;
    window_box_fgcolor = DEFAULT_WINDOW_BOX_FGCOLOR;
    message_history_bgcolor = DEFAULT_WINDOW_BOX_BGCOLOR;
    frame_dirty = true;
}

inline Color gamestate::get_debug_panel_bgcolor() const {
    return Color{window_box_bgcolor.r, window_box_bgcolor.g, window_box_bgcolor.b, 255};
}
