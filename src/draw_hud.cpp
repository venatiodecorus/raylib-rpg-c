#include "draw_hud.h"

#include <array>

void draw_hud(gamestate& g) {
    constexpr int font_size = DEFAULT_HUD_FONT_SIZE;
    constexpr int line_thickness = 2;
    constexpr int line_gap = 2;
    constexpr int line_count = 5;
    constexpr int inner_pad = 8;

    const int turn = g.turn_count;
    const vec2 hp_value = g.ct.get_or<hp>(g.hero_id, vec2{-1, -1});
    const int myhp = hp_value.x;
    const int mymaxhp = hp_value.y;
    const int mylevel = g.ct.get_or<level>(g.hero_id, 0);
    const int myxp = g.ct.get_or<xp>(g.hero_id, 0);
    const int attack_bonus = get_stat_bonus(g.ct.get_or<strength>(g.hero_id, 10));
    const int ac = g.compute_armor_class(g.hero_id);
    const int str = g.ct.get_or<strength>(g.hero_id, -1);
    const int dex = g.ct.get_or<dexterity>(g.hero_id, -1);
    const int con = g.ct.get_or<constitution>(g.hero_id, -1);
    const int int_ = g.ct.get_or<intelligence>(g.hero_id, -1);
    const int wis = g.ct.get_or<wisdom>(g.hero_id, -1);
    const int cha = g.ct.get_or<charisma>(g.hero_id, -1);
    const string n = g.ct.get_or<name>(g.hero_id, "no-name");
    const vec3 loc = g.ct.get_or<location>(g.hero_id, vec3{-1, -1, -1});
    const alignment_t hero_alignment = g.ct.get_or<alignment>(g.hero_id, alignment_t::NONE);
    const std::array<std::string, line_count> lines = {
        n,
        TextFormat("Lvl %d HP %d/%d  Atk: %d  AC: %d", mylevel, myhp, mymaxhp, attack_bonus, ac),
        TextFormat("Str %d Dex %d Con %d Int %d Wis %d Cha %d", str, dex, con, int_, wis, cha),
        TextFormat("Location: (%d, %d, %d) Turn %d  XP %d", loc.x, loc.y, loc.z, turn, myxp),
        TextFormat("Alignment: %s", alignment_to_str(hero_alignment).c_str()),
    };

    int max_w = 0;
    for (const std::string& line : lines) {
        max_w = std::max(max_w, MeasureText(line.c_str(), font_size));
    }

    const float box_w = max_w + inner_pad * 2;
    const float box_h = line_count * font_size + (line_count - 1) * line_gap + inner_pad * 2;
    const float box_x = g.targetwidth / 2.0f - (box_w / 2.0f);
    const float box_y = g.targetheight - box_h - g.pad;

    DrawRectangleRec(Rectangle{box_x, box_y, box_w, box_h}, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(Rectangle{box_x, box_y, box_w, box_h}, line_thickness, g.ui.window_box_fgcolor);

    const int text_x = box_x + inner_pad;
    int text_y = box_y + inner_pad;
    for (const std::string& line : lines) {
        DrawText(line.c_str(), text_x, text_y, font_size, g.ui.window_box_fgcolor);
        text_y += font_size + line_gap;
    }
}
