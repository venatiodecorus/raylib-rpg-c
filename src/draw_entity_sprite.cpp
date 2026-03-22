#include "draw_entity_sprite.h"

#include "draw_shield_sprite.h"
#include "draw_sprite.h"
#include "draw_weapon_sprite.h"
#include "get_equipped_sprite.h"
#include "libdraw_context.h"

void draw_entity_sprite([[maybe_unused]] gamestate& g, spritegroup* sg) {
    massert(sg, "spritegroup is NULL");
    Rectangle dest = {sg->dest.x, sg->dest.y, sg->dest.width, sg->dest.height};
    massert(dest.width > 0, "dest.width is 0");
    massert(dest.height > 0, "dest.height is 0");
    shared_ptr<sprite> s = sg->get_current();
    massert(s, "sprite is NULL");
    DrawTexturePro(*s->get_texture(), s->get_src(), dest, Vector2{0, 0}, 0, WHITE);
#ifdef ENTITY_BORDER
    DrawRectangleLinesEx(dest, 1, Color{255, 0, 0, 255});
#endif
}

void draw_weapon_sprite_back(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) {
    massert(id != ENTITYID_INVALID, "id is invalid");
    massert(sg, "spritegroup is NULL");
    auto weapon_back_s = get_weapon_back_sprite(g, renderer, id, sg);
    if (weapon_back_s) {
        DrawTexturePro(*weapon_back_s->get_texture(), weapon_back_s->get_src(), sg->dest, Vector2{0, 0}, 0, WHITE);
    }
}

void draw_weapon_sprite_front(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) {
    massert(id != ENTITYID_INVALID, "id is invalid");
    massert(sg, "spritegroup is NULL");
    auto weapon_front_s = get_weapon_front_sprite(g, renderer, id, sg);
    if (weapon_front_s) {
        DrawTexturePro(*weapon_front_s->get_texture(), weapon_front_s->get_src(), sg->dest, Vector2{0, 0}, 0, WHITE);
    }
}

void draw_shield_sprite_back(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) {
    massert(id != ENTITYID_INVALID, "id is invalid");
    massert(sg, "spritegroup is NULL");
    auto shield_back_s = get_shield_back_sprite(g, renderer, id, sg);
    if (shield_back_s) {
        DrawTexturePro(*shield_back_s->get_texture(), shield_back_s->get_src(), sg->dest, Vector2{0, 0}, 0, WHITE);
    }
    else {
        merror3("no shield back sprite");
    }
}

void draw_shield_sprite_front(gamestate& g, rpg::Renderer& renderer, entityid id, spritegroup* sg) {
    massert(id != ENTITYID_INVALID, "id is invalid");
    massert(sg, "spritegroup is NULL");
    auto shield_front_s = get_shield_front_sprite(g, renderer, id, sg);
    if (shield_front_s) {
        DrawTexturePro(*shield_front_s->get_texture(), shield_front_s->get_src(), sg->dest, Vector2{0, 0}, 0, WHITE);
    }
    else {
        merror3("no shield front sprite");
    }
}

void draw_sprite_and_shadow(gamestate& g, rpg::Renderer& renderer, entityid id) {
    massert(id != ENTITYID_INVALID, "id is invalid");
    auto it = renderer.spritegroups.find(id);
    massert(it != renderer.spritegroups.end(), "NO SPRITE GROUP FOR ID %d", id);
    spritegroup* sg = it->second.get();
    massert(sg, "sg is NULL");
    draw_shield_sprite_back(g, renderer, id, sg);
    draw_weapon_sprite_back(g, renderer, id, sg);
    draw_entity_sprite(g, sg);
    draw_shield_sprite_front(g, renderer, id, sg);
    draw_weapon_sprite_front(g, renderer, id, sg);
}
