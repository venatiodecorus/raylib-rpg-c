#include "libdraw_from_texture.h"
#include "libdraw_to_texture.h"

#include "draw_character_creation_screen.h"
#include "draw_handle_debug_panel.h"
#include "draw_title_screen.h"
#include "libdraw_context.h"

void draw_char_creation_to_texture(gamestate& g) {
    minfo3("draw char creation to texture");
    BeginTextureMode(libdraw_ctx.char_creation_target_texture);
    draw_character_creation_screen(g);
    handle_debug_panel(g);
    EndTextureMode();
    msuccess3("draw char creation to texture");
}

void draw_title_screen_to_texture(gamestate& g, bool show_menu) {
    BeginTextureMode(libdraw_ctx.title_target_texture);
    draw_title_screen(g, show_menu);
    handle_debug_panel(g);
    EndTextureMode();
}

void draw_title_screen_from_texture(gamestate& g) {
    (void)g;
    DrawTexturePro(libdraw_ctx.title_target_texture.texture, libdraw_ctx.target_src, libdraw_ctx.target_dest, Vector2{0, 0}, 0.0f, WHITE);
}

void draw_char_creation_from_texture(gamestate& g) {
    (void)g;
    DrawTexturePro(libdraw_ctx.char_creation_target_texture.texture, libdraw_ctx.target_src, libdraw_ctx.target_dest, Vector2{0, 0}, 0.0f, WHITE);
}
