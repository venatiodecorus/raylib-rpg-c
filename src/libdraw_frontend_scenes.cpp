#include "libdraw_from_texture.h"
#include "libdraw_to_texture.h"

#include "draw_character_creation_screen.h"
#include "draw_handle_debug_panel.h"
#include "draw_title_screen.h"
#include "libdraw_context.h"

void draw_char_creation_to_texture(gamestate& g, rpg::Renderer& renderer) {
    minfo3("draw char creation to texture");
    BeginTextureMode(renderer.char_creation_target_texture);
    draw_character_creation_screen(g, renderer);
    handle_debug_panel(g);
    EndTextureMode();
    msuccess3("draw char creation to texture");
}

void draw_title_screen_to_texture(gamestate& g, rpg::Renderer& renderer, bool show_menu) {
    BeginTextureMode(renderer.title_target_texture);
    draw_title_screen(g, renderer, show_menu);
    handle_debug_panel(g);
    EndTextureMode();
}

void draw_title_screen_from_texture([[maybe_unused]] gamestate& g, rpg::Renderer& renderer) {
    DrawTexturePro(renderer.title_target_texture.texture, renderer.target_src, renderer.target_dest, Vector2{0, 0}, 0.0f, WHITE);
}

void draw_char_creation_from_texture([[maybe_unused]] gamestate& g, rpg::Renderer& renderer) {
    DrawTexturePro(renderer.char_creation_target_texture.texture, renderer.target_src, renderer.target_dest, Vector2{0, 0}, 0.0f, WHITE);
}
