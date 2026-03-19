#include "draw_frame_2d.h"

#include "camera_lock_on.h"
#include "draw_action_menu.h"
#include "draw_chest_menu.h"
#include "draw_controls_menu.h"
#include "draw_damage_numbers.h"
#include "draw_dungeon_floor.h"
#include "draw_handle_debug_panel.h"
#include "draw_help_menu.h"
#include "draw_hud.h"
#include "draw_interaction_modal.h"
#include "draw_inventory_menu.h"
#include "draw_keyboard_profile_prompt.h"
#include "draw_level_up_modal.h"
#include "draw_look_panel.h"
#include "draw_message_box.h"
#include "draw_message_history.h"
#include "draw_mini_inventory_menu.h"
#include "draw_option_menu.h"
#include "draw_sound_menu.h"
#include "draw_window_color_menu.h"
#include "libdraw_context.h"
#include "libdraw_from_texture.h"
#include "libdraw_to_texture.h"

void draw_hud_to_texture(gamestate& g, rpg::Renderer& renderer) {
    BeginTextureMode(renderer.hud_target_texture);
    draw_hud(g);
    EndTextureMode();
}

void libdraw_drawframe_2d(gamestate& g, rpg::Renderer& renderer, int vision_dist, int light_rad) {
    camera_lock_on(g, renderer);

    BeginMode2D(g.cam2d);
    ClearBackground(BLACK);
    draw_dungeon_floor(g, renderer, vision_dist, light_rad);
    draw_damage_numbers(g);
    EndMode2D();

    draw_hud(g);
    draw_look_panel(g);
    draw_message_history(g);
    draw_message_box(g);
    draw_interaction_modal(g);
    draw_level_up_modal(g);

    if (g.ui.display_inventory_menu) {
        if (g.use_mini_inventory_menu()) {
            auto maybe_inventory = g.ct.get<inventory>(g.hero_id);
            auto items = maybe_inventory.value_or(nullptr);
            if (items) {
                draw_mini_inventory_menu(g, renderer, items, "Inventory", "E equip  Enter use  Q drop  Esc close", true);
            }
        }
        else {
            draw_inventory_menu(g, renderer);
        }
    }

    if (g.ui.display_chest_menu) {
        if (g.use_mini_inventory_menu()) {
            const entityid source_id = g.ui.chest_deposit_mode ? g.hero_id : g.active_chest_id;
            auto maybe_inventory = g.ct.get<inventory>(source_id);
            auto items = maybe_inventory.value_or(nullptr);
            if (items) {
                draw_mini_inventory_menu(
                    g, renderer,
                    items,
                    g.ui.chest_deposit_mode ? "Chest Deposit" : "Chest",
                    g.ui.chest_deposit_mode ? "Tab chest  Enter deposit  D close" : "Tab hero  Enter take  D close",
                    g.ui.chest_deposit_mode);
            }
        }
        else {
            draw_chest_menu(g, renderer);
        }
    }

    if (g.ui.display_action_menu) {
        draw_action_menu(g);
    }
    if (g.ui.display_option_menu) {
        draw_option_menu(g);
    }
    if (g.ui.display_sound_menu) {
        draw_sound_menu(g);
    }
    if (g.ui.display_window_color_menu) {
        draw_window_color_menu(g);
    }
    if (g.ui.display_controls_menu) {
        draw_controls_menu(g);
    }
    if (g.ui.display_help_menu) {
        draw_help_menu(g);
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

void draw_hud_from_texture(gamestate& g, rpg::Renderer& renderer) {
    (void)g;
    DrawTexturePro(renderer.hud_target_texture.texture, renderer.target_src, renderer.target_dest, Vector2{0, 0}, 0.0f, WHITE);
}

void libdraw_drawframe_2d_from_texture(gamestate& g, rpg::Renderer& renderer) {
    (void)g;
    DrawTexturePro(renderer.main_game_target_texture.texture, renderer.target_src, renderer.target_dest, Vector2{0, 0}, 0.0f, WHITE);
}
