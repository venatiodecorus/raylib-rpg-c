/** @file draw_title_screen.h
 *  @brief Title-screen and main-menu drawing helper.
 */

#pragma once

#include "gamestate.h"
#include "libdraw_context.h"
#include "actor_definitions.h"
#include "item_definitions.h"
#include <raylib.h>

/** @brief Draw the title screen and optionally its menu selection list. */
void draw_title_screen(gamestate& g, rpg::Renderer& renderer, bool show_menu);
