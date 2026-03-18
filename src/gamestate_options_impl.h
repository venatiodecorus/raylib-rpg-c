/** @file gamestate_options_impl.h
 *  @brief Audio and window-color options helpers implemented on `gamestate`.
 */

#pragma once

#include "libdraw_context.h"
#include <algorithm>

inline void gamestate::open_sound_menu() {
    ui.display_option_menu = false;
    ui.display_sound_menu = true;
    ui.sound_menu_selection = 0;
    controlmode = CONTROLMODE_SOUND_MENU;
    frame_dirty = true;
}

inline void gamestate::close_sound_menu() {
    ui.display_sound_menu = false;
    ui.display_option_menu = true;
    controlmode = CONTROLMODE_OPTION_MENU;
    frame_dirty = true;
}

inline void gamestate::open_window_color_menu() {
    ui.display_option_menu = false;
    ui.display_window_color_menu = true;
    ui.window_color_menu_selection = 0;
    controlmode = CONTROLMODE_WINDOW_COLOR_MENU;
    frame_dirty = true;
}

inline void gamestate::close_window_color_menu() {
    ui.display_window_color_menu = false;
    ui.display_option_menu = true;
    controlmode = CONTROLMODE_OPTION_MENU;
    frame_dirty = true;
}

inline void gamestate::adjust_window_box_bg_channel(size_t channel, int dir) {
    unsigned char* values[] = {&ui.window_box_bgcolor.r, &ui.window_box_bgcolor.g, &ui.window_box_bgcolor.b, &ui.window_box_bgcolor.a};
    if (channel >= 4) {
        return;
    }
    const int adjusted = std::clamp(static_cast<int>(*values[channel]) + dir, 0, 255);
    *values[channel] = static_cast<unsigned char>(adjusted);
    ui.message_history_bgcolor = ui.window_box_bgcolor;
    frame_dirty = true;
}

inline void gamestate::adjust_window_box_fg_channel(size_t channel, int dir) {
    unsigned char* values[] = {&ui.window_box_fgcolor.r, &ui.window_box_fgcolor.g, &ui.window_box_fgcolor.b, &ui.window_box_fgcolor.a};
    if (channel >= 4) {
        return;
    }
    const int adjusted = std::clamp(static_cast<int>(*values[channel]) + dir, 0, 255);
    *values[channel] = static_cast<unsigned char>(adjusted);
    frame_dirty = true;
}

inline void gamestate::reset_window_box_colors() {
    ui.window_box_bgcolor = DEFAULT_WINDOW_BOX_BGCOLOR;
    ui.window_box_fgcolor = DEFAULT_WINDOW_BOX_FGCOLOR;
    ui.message_history_bgcolor = DEFAULT_WINDOW_BOX_BGCOLOR;
    frame_dirty = true;
}

inline Color gamestate::get_debug_panel_bgcolor() const {
    return Color{ui.window_box_bgcolor.r, ui.window_box_bgcolor.g, ui.window_box_bgcolor.b, 255};
}
