/** @file draw_handle_debug_panel.h
 *  @brief Debug-panel update-and-draw orchestration helper.
 */

#pragma once

#include "gamestate.h"
#include "libdraw_context.h"
#include "spritegroup.h"

/** @brief Draw the on-screen debug panel using the current buffer contents. */
void draw_debug_panel(gamestate& g);

/** @brief Append the current debug text payload into the debug panel buffer. */
void update_debug_panel(gamestate& g);

/** @brief Refresh and draw the debug panel when it is enabled. */
void handle_debug_panel(gamestate& g);
