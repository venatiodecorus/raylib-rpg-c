/** @file libdraw_gameplay_scene.cpp
 *  @brief Gameplay scene rendering composition using renderer classes.
 */

#include "camera_lock_on.h"
#include "ecs_gameplay_components.h"
#include "hud_renderer.h"
#include "inventory_renderer.h"
#include "libdraw_context.h"
#include "libdraw_from_texture.h"
#include "libdraw_to_texture.h"
#include "menu_renderer.h"
#include "world_renderer.h"

#include "draw_handle_debug_panel.h"
#include "draw_keyboard_profile_prompt.h"

static rpg::WorldRenderer world_renderer;
static rpg::HudRenderer hud_renderer;
static rpg::MenuRenderer menu_renderer;
static rpg::InventoryRenderer inventory_renderer;

void draw_hud_to_texture(gamestate& g, rpg::Renderer& renderer) {
    BeginTextureMode(renderer.hud_target_texture);
    hud_renderer.draw_hud(g);
    EndTextureMode();
}

void libdraw_drawframe_2d(gamestate& g, rpg::Renderer& renderer, int vision_dist, int light_rad) {
    camera_lock_on(g, renderer);

    BeginMode2D(g.cam2d);
    ClearBackground(BLACK);
    world_renderer.draw_floor(g, renderer, vision_dist, light_rad);
    world_renderer.draw_damage_numbers(g);
    EndMode2D();

    hud_renderer.draw_hud(g);
    hud_renderer.draw_look_panel(g);
    hud_renderer.draw_message_history(g);
    hud_renderer.draw_message_box(g);
    hud_renderer.draw_interaction_modal(g);
    hud_renderer.draw_level_up_modal(g);

    if (g.ui.display_inventory_menu) {
        if (g.use_mini_inventory_menu()) {
            const Inventory* items = g.get_component<Inventory>(g.hero_id);
            if (items) {
                inventory_renderer.draw_mini_inventory(g, renderer, items->value, "Inventory", "E equip  Enter use  Q drop  Esc close", true);
            }
        }
        else {
            inventory_renderer.draw_inventory(g, renderer);
        }
    }

    if (g.ui.display_chest_menu) {
        if (g.use_mini_inventory_menu()) {
            const entityid source_id = g.ui.chest_deposit_mode ? g.hero_id : g.active_chest_id;
            const Inventory* items = g.get_component<Inventory>(source_id);
            if (items) {
                inventory_renderer.draw_mini_inventory(
                    g,
                    renderer,
                    items->value,
                    g.ui.chest_deposit_mode ? "Chest Deposit" : "Chest",
                    g.ui.chest_deposit_mode ? "Tab chest  Enter deposit  D close" : "Tab hero  Enter take  D close",
                    g.ui.chest_deposit_mode);
            }
        }
        else {
            inventory_renderer.draw_chest(g, renderer);
        }
    }

    if (g.ui.display_action_menu) {
        menu_renderer.draw_action_menu(g);
    }
    if (g.ui.display_option_menu) {
        menu_renderer.draw_option_menu(g);
    }
    if (g.ui.display_sound_menu) {
        menu_renderer.draw_sound_menu(g);
    }
    if (g.ui.display_window_color_menu) {
        menu_renderer.draw_window_color_menu(g);
    }
    if (g.ui.display_controls_menu) {
        menu_renderer.draw_controls_menu(g);
    }
    if (g.ui.display_help_menu) {
        menu_renderer.draw_help_menu(g);
    }

    draw_keyboard_profile_prompt(g);
#ifdef DEBUG
    handle_debug_panel(g);
#endif
}

void libdraw_drawframe_2d_to_texture(gamestate& g, rpg::Renderer& renderer, int vision_dist, int light_rad) {
    BeginTextureMode(renderer.main_game_target_texture);
    libdraw_drawframe_2d(g, renderer, vision_dist, light_rad);
    EndTextureMode();
}

void draw_hud_from_texture([[maybe_unused]] gamestate& g, rpg::Renderer& renderer) {
    DrawTexturePro(renderer.hud_target_texture.texture, renderer.target_src, renderer.target_dest, Vector2{0, 0}, 0.0f, WHITE);
}

void libdraw_drawframe_2d_from_texture([[maybe_unused]] gamestate& g, rpg::Renderer& renderer) {
    DrawTexturePro(renderer.main_game_target_texture.texture, renderer.target_src, renderer.target_dest, Vector2{0, 0}, 0.0f, WHITE);
}
