#pragma once

/** @file libdraw.h
 *  @brief Top-level composition include for the renderer subsystem.
 */

#include "libdraw_context.h"

class gamestate;

void libdraw_render_current_scene_to_scene_texture(gamestate& g, rpg::Renderer& renderer);
void libdraw_draw_current_scene_from_scene_texture(gamestate& g, rpg::Renderer& renderer);
void libdraw_refresh_dirty_scene(gamestate& g, rpg::Renderer& renderer);
void libdraw_compose_scene_to_window_target(gamestate& g, rpg::Renderer& renderer);
void libdraw_present_window_target(gamestate& g, rpg::Renderer& renderer);
void drawframe(gamestate& g, rpg::Renderer& renderer);
void libdraw_init_render_targets(gamestate& g, rpg::Renderer& renderer);
void libdraw_init_resources(gamestate& g, rpg::Renderer& renderer);
void libdraw_init_rest(gamestate& g, rpg::Renderer& renderer);
void libdraw_init(gamestate& g, rpg::Renderer& renderer);
bool libdraw_windowshouldclose(gamestate& g);
void libdraw_close_partial(rpg::Renderer& renderer);
void libdraw_close(rpg::Renderer& renderer);
