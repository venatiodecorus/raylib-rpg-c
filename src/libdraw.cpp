/** @file libdraw.cpp
 *  @brief Top-level renderer entry points and process-lifetime renderer globals.
 */

#include "libdraw.h"
#include "ComponentTraits.h"
#include "create_sg_byid.h"
#include "draw_action_menu.h"
#include "draw_character_creation_screen.h"
#include "draw_chest_menu.h"
#include "draw_controls_menu.h"
#include "draw_damage_numbers.h"
#include "draw_dungeon_floor.h"
#include "draw_entity_sprite.h"
#include "draw_frame_2d.h"
#include "draw_shield_sprite.h"
#include "draw_sprite.h"
#include "draw_weapon_sprite.h"
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
#include "draw_title_screen.h"
#include "draw_window_color_menu.h"
#include "entitytype.h"
#include "libdraw_frame_stats.h"
#include "libdraw_from_texture.h"
#include "libdraw_update_shield_for_entity.h"
#include "libdraw_to_texture.h"
#include "libdraw_update_sprites.h"
#include "libdraw_update_weapon_for_entity.h"
#include "load_textures.h"
#include "set_sg.h"
#include "shaders.h"
#include "spritegroup_anim.h"
#include "stat_bonus.h"
#include "unload_textures.h"
#include "update_sprite.h"
#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>
#include <memory>

libdraw_context_t libdraw_ctx;



void draw_keyboard_profile_prompt(gamestate& g) {
    if (!g.ui.display_keyboard_profile_prompt) {
        return;
    }

    const int box_w = 420;
    const int box_h = 150;
    const int box_x = (DEFAULT_TARGET_WIDTH - box_w) / 2;
    const int box_y = (DEFAULT_TARGET_HEIGHT - box_h) / 2;
    const Rectangle box = {(float)box_x, (float)box_y, (float)box_w, (float)box_h};

    DrawRectangleRec(box, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(box, 2.0f, g.ui.window_box_fgcolor);
    DrawText("Choose Keyboard Layout", box_x + 16, box_y + 14, 20, g.ui.window_box_fgcolor);
    DrawText("Select a default gameplay profile before entering the dungeon.", box_x + 16, box_y + 42, 10, g.ui.window_box_fgcolor);
    DrawText("You can change this later in ` -> controls.", box_x + 16, box_y + 56, 10, g.ui.window_box_fgcolor);

    for (int i = 0; i < KEYBOARD_PROFILE_COUNT; i++) {
        const bool selected = g.ui.keyboard_profile_selection == (unsigned int)i;
        const int option_y = box_y + 88 + i * 18;
        const char* label = keyboard_profile_label(static_cast<keyboard_profile_t>(i));
        DrawText(selected ? TextFormat("> %s", label) : TextFormat("  %s", label), box_x + 24, option_y, 12, selected ? YELLOW : g.ui.window_box_fgcolor);
    }

    DrawText("Arrows to choose, Enter to confirm", box_x + 16, box_y + box_h - 20, 10, g.ui.window_box_fgcolor);
}

void libdraw_render_current_scene_to_scene_texture(gamestate& g) {
    switch (g.current_scene) {
    case SCENE_TITLE:
        draw_title_screen_to_texture(g, false);
        break;
    case SCENE_MAIN_MENU:
        draw_title_screen_to_texture(g, true);
        break;
    case SCENE_CHARACTER_CREATION:
        minfo3("draw character creation scene to texture");
        draw_char_creation_to_texture(g);
        break;
    case SCENE_GAMEPLAY: {
        const int vision_dist = g.ct.get<vision_distance>(g.hero_id).value_or(0);
        const int light_rad = g.ct.get<light_radius>(g.hero_id).value_or(0);
        libdraw_drawframe_2d_to_texture(g, vision_dist, light_rad);
    } break;
    default:
        break;
    }
}

void libdraw_draw_current_scene_from_scene_texture(gamestate& g) {
    switch (g.current_scene) {
    case SCENE_TITLE:
    case SCENE_MAIN_MENU:
        draw_title_screen_from_texture(g);
        break;
    case SCENE_CHARACTER_CREATION:
        draw_char_creation_from_texture(g);
        break;
    case SCENE_GAMEPLAY:
        libdraw_drawframe_2d_from_texture(g);
        break;
    default:
        break;
    }
}

void libdraw_refresh_dirty_scene(gamestate& g) {
    if (!g.frame_dirty) {
        return;
    }

    minfo3("frame is dirty");
    libdraw_render_current_scene_to_scene_texture(g);
    g.frame_dirty = false;
    g.frame_updates++;
    msuccess3("frame is no longer dirty");
}

void libdraw_compose_scene_to_window_target(gamestate& g) {
    BeginTextureMode(libdraw_ctx.target);
    ClearBackground(BLUE);
    libdraw_draw_current_scene_from_scene_texture(g);
    EndTextureMode();
}

void libdraw_present_window_target(gamestate& g) {
    (void)g;
    libdraw_ctx.win_dest.width = GetScreenWidth();
    libdraw_ctx.win_dest.height = GetScreenHeight();
    DrawTexturePro(libdraw_ctx.target.texture, libdraw_ctx.target_src, libdraw_ctx.win_dest, Vector2{0, 0}, 0.0f, WHITE);
    DrawFPS(10, 10);
}

void drawframe(gamestate& g) {
    minfo3("drawframe");
    const double start_time = libdraw_frame_begin_time();
    libdraw_update_sprites_pre(g);

    BeginDrawing();
    ClearBackground(RED);
    libdraw_refresh_dirty_scene(g);
    libdraw_compose_scene_to_window_target(g);
    libdraw_present_window_target(g);
    EndDrawing();
    libdraw_finish_frame_stats(g, start_time);

    libdraw_update_sprites_post(g);
    msuccess3("drawframe");
}

void libdraw_init_render_targets(gamestate& g) {
    g.windowwidth = DEFAULT_WIN_WIDTH;
    g.windowheight = DEFAULT_WIN_HEIGHT;
    g.targetwidth = DEFAULT_TARGET_WIDTH;
    g.targetheight = DEFAULT_TARGET_HEIGHT;

    const TextureFilter filter = TEXTURE_FILTER_POINT;
    libdraw_ctx.target = LoadRenderTexture(DEFAULT_TARGET_WIDTH, DEFAULT_TARGET_HEIGHT);
    libdraw_ctx.title_target_texture = LoadRenderTexture(DEFAULT_TARGET_WIDTH, DEFAULT_TARGET_HEIGHT);
    libdraw_ctx.char_creation_target_texture = LoadRenderTexture(DEFAULT_TARGET_WIDTH, DEFAULT_TARGET_HEIGHT);
    libdraw_ctx.main_game_target_texture = LoadRenderTexture(DEFAULT_TARGET_WIDTH, DEFAULT_TARGET_HEIGHT);
    libdraw_ctx.hud_target_texture = LoadRenderTexture(DEFAULT_TARGET_WIDTH, DEFAULT_TARGET_HEIGHT);

    SetTextureFilter(libdraw_ctx.target.texture, filter);
    SetTextureFilter(libdraw_ctx.title_target_texture.texture, filter);
    SetTextureFilter(libdraw_ctx.char_creation_target_texture.texture, filter);
    SetTextureFilter(libdraw_ctx.main_game_target_texture.texture, filter);
    SetTextureFilter(libdraw_ctx.hud_target_texture.texture, filter);

    libdraw_ctx.target_src = Rectangle{0, 0, DEFAULT_TARGET_WIDTH * 1.0f, -DEFAULT_TARGET_HEIGHT * 1.0f};
    libdraw_ctx.target_dest = Rectangle{0, 0, DEFAULT_TARGET_WIDTH * 1.0f, DEFAULT_TARGET_HEIGHT * 1.0f};
}

void libdraw_init_resources(gamestate& g) {
    load_textures(libdraw_ctx.txinfo);
    load_shaders();

    const float x = DEFAULT_TARGET_WIDTH / 4.0f;
    const float y = DEFAULT_TARGET_HEIGHT / 4.0f;
    g.cam2d.offset = Vector2{x, y};

    draw_title_screen_to_texture(g, false);
    draw_char_creation_to_texture(g);

}

void libdraw_init_rest(gamestate& g) {
    SetExitKey(KEY_ESCAPE);
    SetTargetFPS(60);
    libdraw_init_render_targets(g);
    libdraw_init_resources(g);
}

void libdraw_init(gamestate& g) {
    const int w = DEFAULT_WIN_WIDTH;
    const int h = DEFAULT_WIN_HEIGHT;
    const char* title = WINDOW_TITLE;
    char full_title[1024] = {0};
    snprintf(full_title, sizeof(full_title), "%s - %s", title, g.version.c_str());
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(w, h, full_title);
    g.windowwidth = w;
    g.windowheight = h;
    SetWindowMinSize(320, 240);
    libdraw_init_rest(g);
}

bool libdraw_windowshouldclose(gamestate& g) {
    return g.do_quit;
}

void libdraw_close_partial() {
    libdraw_ctx.spritegroups.clear();
    libdraw_unload_textures(libdraw_ctx.txinfo);
    unload_shaders();
    UnloadRenderTexture(libdraw_ctx.title_target_texture);
    UnloadRenderTexture(libdraw_ctx.char_creation_target_texture);
    UnloadRenderTexture(libdraw_ctx.main_game_target_texture);
    UnloadRenderTexture(libdraw_ctx.hud_target_texture);
    UnloadRenderTexture(libdraw_ctx.target);
}

void libdraw_close() {
    libdraw_close_partial();
    CloseWindow();
}
