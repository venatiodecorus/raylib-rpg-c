/** @file libdraw.cpp
 *  @brief Top-level renderer entry points and process-lifetime renderer globals.
 */

#include "libdraw.h"
#include "draw_character_creation_screen.h"
#include "draw_handle_debug_panel.h"
#include "draw_keyboard_profile_prompt.h"
#include "draw_title_screen.h"
#include "ecs_gameplay_components.h"
#include "entitytype.h"
#include "libdraw_frame_stats.h"
#include "libdraw_from_texture.h"
#include "libdraw_to_texture.h"
#include "libdraw_update_shield_for_entity.h"
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

Texture2D rpg::Renderer::load_sprite_texture(const rpg::SpriteDef& def) {
    std::string cache_key;
    switch (def.src_type) {
    case rpg::TXSRC_FILE: cache_key = def.path; break;
    case rpg::TXSRC_PLACEHOLDER:
        cache_key = "__placeholder_" + std::to_string(def.src_w > 0 ? def.src_w : 32) + "x" + std::to_string(def.src_h > 0 ? def.src_h : 32);
        break;
    case rpg::TXSRC_TILEMAP:
        cache_key =
            def.path + "@" + std::to_string(def.src_x) + "," + std::to_string(def.src_y) + "," + std::to_string(def.src_w) + "," + std::to_string(def.src_h);
        break;
    }

    auto it = texture_cache.find(cache_key);
    if (it != texture_cache.end()) {
        return it->second;
    }

    Image image = {};
    switch (def.src_type) {
    case rpg::TXSRC_FILE: {
        image = LoadImage(def.path.c_str());
        massert(image.data != NULL, "Failed to load sprite image: %s", def.path.c_str());
        // Trim transparent padding from sprite sheet frames
        if (def.frames > 0 && def.contexts > 0) {
            int cell_w = image.width / def.frames;
            int cell_h = image.height / def.contexts;
            if (cell_w > 8 || cell_h > 8) {
                // Find union content rect across ALL cells
                int u_min_x = cell_w, u_min_y = cell_h, u_max_x = -1, u_max_y = -1;
                for (int ctx = 0; ctx < def.contexts; ctx++) {
                    for (int fr = 0; fr < def.frames; fr++) {
                        for (int py = 0; py < cell_h; py++) {
                            for (int px = 0; px < cell_w; px++) {
                                Color c = GetImageColor(image, fr * cell_w + px, ctx * cell_h + py);
                                if (c.a > 0) {
                                    if (px < u_min_x)
                                        u_min_x = px;
                                    if (py < u_min_y)
                                        u_min_y = py;
                                    if (px > u_max_x)
                                        u_max_x = px;
                                    if (py > u_max_y)
                                        u_max_y = py;
                                }
                            }
                        }
                    }
                }
                if (u_max_x >= u_min_x && u_max_y >= u_min_y) {
                    int cw = u_max_x - u_min_x + 1;
                    int ch = u_max_y - u_min_y + 1;
                    Image compact = GenImageColor(cw * def.frames, ch * def.contexts, Color{0, 0, 0, 0});
                    for (int ctx = 0; ctx < def.contexts; ctx++) {
                        for (int fr = 0; fr < def.frames; fr++) {
                            Rectangle src_r = {
                                static_cast<float>(fr * cell_w + u_min_x),
                                static_cast<float>(ctx * cell_h + u_min_y),
                                static_cast<float>(cw),
                                static_cast<float>(ch)};
                            Rectangle dst_r = {static_cast<float>(fr * cw), static_cast<float>(ctx * ch), static_cast<float>(cw), static_cast<float>(ch)};
                            Image cell = ImageFromImage(image, src_r);
                            ImageDraw(&compact, cell, Rectangle{0, 0, static_cast<float>(cw), static_cast<float>(ch)}, dst_r, WHITE);
                            UnloadImage(cell);
                        }
                    }
                    UnloadImage(image);
                    image = compact;
                }
            }
        }
        break;
    }
    case rpg::TXSRC_PLACEHOLDER: {
        int w = (def.src_w > 0) ? def.src_w : 32;
        int h = (def.src_h > 0) ? def.src_h : 32;
        image = GenImageColor(w, h, MAGENTA);
        break;
    }
    case rpg::TXSRC_TILEMAP: {
        Image tilemap = LoadImage(def.path.c_str());
        massert(tilemap.data != NULL, "Failed to load tilemap: %s", def.path.c_str());
        Rectangle src_rect = {static_cast<float>(def.src_x), static_cast<float>(def.src_y), static_cast<float>(def.src_w), static_cast<float>(def.src_h)};
        image = ImageFromImage(tilemap, src_rect);
        UnloadImage(tilemap);
        break;
    }
    }

    Texture2D tex = LoadTextureFromImage(image);
    UnloadImage(image);
    texture_cache[cache_key] = tex;
    return tex;
}

void rpg::Renderer::unload_texture_cache() {
    for (auto& pair : texture_cache) {
        UnloadTexture(pair.second);
    }
    texture_cache.clear();
}

void draw_keyboard_profile_prompt(gamestate& g) {
    if (!g.ui.display_keyboard_profile_prompt) {
        return;
    }

    const int box_w = 420;
    const int box_h = 150;
    const int box_x = (DEFAULT_TARGET_WIDTH - box_w) / 2;
    const int box_y = (DEFAULT_TARGET_HEIGHT - box_h) / 2;
    const Rectangle box = {static_cast<float>(box_x), static_cast<float>(box_y), static_cast<float>(box_w), static_cast<float>(box_h)};

    DrawRectangleRec(box, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(box, 2.0f, g.ui.window_box_fgcolor);
    DrawText("Choose Keyboard Layout", box_x + 16, box_y + 14, 20, g.ui.window_box_fgcolor);
    DrawText("Select a default gameplay profile before entering the dungeon.", box_x + 16, box_y + 42, 10, g.ui.window_box_fgcolor);
    DrawText("You can change this later in ` -> controls.", box_x + 16, box_y + 56, 10, g.ui.window_box_fgcolor);

    for (int i = 0; i < static_cast<int>(keyboard_profile_t::COUNT); i++) {
        const bool selected = g.ui.keyboard_profile_selection == static_cast<unsigned int>(i);
        const int option_y = box_y + 88 + i * 18;
        const char* label = keyboard_profile_label(static_cast<keyboard_profile_t>(i));
        DrawText(selected ? TextFormat("> %s", label) : TextFormat("  %s", label), box_x + 24, option_y, 12, selected ? YELLOW : g.ui.window_box_fgcolor);
    }

    DrawText("Arrows to choose, Enter to confirm", box_x + 16, box_y + box_h - 20, 10, g.ui.window_box_fgcolor);
}

void libdraw_render_current_scene_to_scene_texture(gamestate& g, rpg::Renderer& renderer) {
    switch (g.current_scene) {
    case scene_t::TITLE: draw_title_screen_to_texture(g, renderer, false); break;
    case scene_t::MAIN_MENU: draw_title_screen_to_texture(g, renderer, true); break;
    case scene_t::CHARACTER_CREATION:
        minfo3("draw character creation scene to texture");
        draw_char_creation_to_texture(g, renderer);
        break;
    case scene_t::GAMEPLAY: {
        const int vision_dist = g.get_component_or<VisionRange>(g.hero_id, 0);
        const int light_rad = g.get_component_or<LightRadius>(g.hero_id, 0);
        libdraw_drawframe_2d_to_texture(g, renderer, vision_dist, light_rad);
    } break;
    default: break;
    }
}

void libdraw_draw_current_scene_from_scene_texture(gamestate& g, rpg::Renderer& renderer) {
    switch (g.current_scene) {
    case scene_t::TITLE:
    case scene_t::MAIN_MENU: draw_title_screen_from_texture(g, renderer); break;
    case scene_t::CHARACTER_CREATION: draw_char_creation_from_texture(g, renderer); break;
    case scene_t::GAMEPLAY: libdraw_drawframe_2d_from_texture(g, renderer); break;
    default: break;
    }
}

void libdraw_refresh_dirty_scene(gamestate& g, rpg::Renderer& renderer) {
    if (!g.frame_dirty) {
        return;
    }

    minfo3("frame is dirty");
    libdraw_render_current_scene_to_scene_texture(g, renderer);
    g.frame_dirty = false;
    g.frame_updates++;
    msuccess3("frame is no longer dirty");
}

void libdraw_compose_scene_to_window_target(gamestate& g, rpg::Renderer& renderer) {
    BeginTextureMode(renderer.target);
    ClearBackground(BLUE);
    libdraw_draw_current_scene_from_scene_texture(g, renderer);
    EndTextureMode();
}

void libdraw_present_window_target([[maybe_unused]] gamestate& g, rpg::Renderer& renderer) {
    renderer.win_dest.width = GetScreenWidth();
    renderer.win_dest.height = GetScreenHeight();
    DrawTexturePro(renderer.target.texture, renderer.target_src, renderer.win_dest, Vector2{0, 0}, 0.0f, WHITE);
    DrawFPS(10, 10);
}

void drawframe(gamestate& g, rpg::Renderer& renderer) {
    minfo3("drawframe");
    const double start_time = libdraw_frame_begin_time();
    libdraw_update_sprites_pre(g, renderer);

    BeginDrawing();
    ClearBackground(RED);
    libdraw_refresh_dirty_scene(g, renderer);
    libdraw_compose_scene_to_window_target(g, renderer);
    libdraw_present_window_target(g, renderer);
    EndDrawing();
    libdraw_finish_frame_stats(g, start_time);

    libdraw_update_sprites_post(g, renderer);
    msuccess3("drawframe");
}

void libdraw_init_render_targets(gamestate& g, rpg::Renderer& renderer) {
    g.windowwidth = DEFAULT_WIN_WIDTH;
    g.windowheight = DEFAULT_WIN_HEIGHT;
    g.targetwidth = DEFAULT_TARGET_WIDTH;
    g.targetheight = DEFAULT_TARGET_HEIGHT;

    const TextureFilter filter = TEXTURE_FILTER_POINT;
    renderer.target = LoadRenderTexture(DEFAULT_TARGET_WIDTH, DEFAULT_TARGET_HEIGHT);
    renderer.title_target_texture = LoadRenderTexture(DEFAULT_TARGET_WIDTH, DEFAULT_TARGET_HEIGHT);
    renderer.char_creation_target_texture = LoadRenderTexture(DEFAULT_TARGET_WIDTH, DEFAULT_TARGET_HEIGHT);
    renderer.main_game_target_texture = LoadRenderTexture(DEFAULT_TARGET_WIDTH, DEFAULT_TARGET_HEIGHT);
    renderer.hud_target_texture = LoadRenderTexture(DEFAULT_TARGET_WIDTH, DEFAULT_TARGET_HEIGHT);

    SetTextureFilter(renderer.target.texture, filter);
    SetTextureFilter(renderer.title_target_texture.texture, filter);
    SetTextureFilter(renderer.char_creation_target_texture.texture, filter);
    SetTextureFilter(renderer.main_game_target_texture.texture, filter);
    SetTextureFilter(renderer.hud_target_texture.texture, filter);

    renderer.target_src = Rectangle{0, 0, DEFAULT_TARGET_WIDTH * 1.0f, -DEFAULT_TARGET_HEIGHT * 1.0f};
    renderer.target_dest = Rectangle{0, 0, DEFAULT_TARGET_WIDTH * 1.0f, DEFAULT_TARGET_HEIGHT * 1.0f};
}

void libdraw_init_resources(gamestate& g, rpg::Renderer& renderer) {
    load_textures(renderer.txinfo);
    load_shaders(renderer);

    const float x = DEFAULT_TARGET_WIDTH / 4.0f;
    const float y = DEFAULT_TARGET_HEIGHT / 4.0f;
    g.cam2d.offset = Vector2{x, y};

    draw_title_screen_to_texture(g, renderer, false);
    draw_char_creation_to_texture(g, renderer);
}

void libdraw_init_rest(gamestate& g, rpg::Renderer& renderer) {
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);
    libdraw_init_render_targets(g, renderer);
    libdraw_init_resources(g, renderer);
}

void libdraw_init(gamestate& g, rpg::Renderer& renderer) {
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
    libdraw_init_rest(g, renderer);
}

bool libdraw_windowshouldclose(gamestate& g) {
    return g.do_quit || WindowShouldClose();
}

void libdraw_close_partial(rpg::Renderer& renderer) {
    renderer.spritegroups.clear();
    libdraw_unload_textures(renderer.txinfo);
    renderer.unload_texture_cache();
    unload_shaders(renderer);
    UnloadRenderTexture(renderer.title_target_texture);
    UnloadRenderTexture(renderer.char_creation_target_texture);
    UnloadRenderTexture(renderer.main_game_target_texture);
    UnloadRenderTexture(renderer.hud_target_texture);
    UnloadRenderTexture(renderer.target);
}

void libdraw_close(rpg::Renderer& renderer) {
    libdraw_close_partial(renderer);
    CloseWindow();
}
