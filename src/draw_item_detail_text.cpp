#include "draw_item_detail_text.h"

vector<string> build_item_detail_lines(gamestate& g, entityid selection_id) {
    vector<string> lines;
    const itemtype_t item_type = g.ct.get_or<itemtype>(selection_id, itemtype_t::NONE);

    lines.push_back(g.ct.get_or<name>(selection_id, "no-name"));

    if (item_type == itemtype_t::WEAPON) {
        const vec3 dmg = g.ct.get_or<damage>(selection_id, vec3{-1, -1, -1});
        lines.push_back(TextFormat("Damage: %d-%d", dmg.x, dmg.y));

        const int dura = g.ct.get_or<durability>(selection_id, -1);
        const int max_dura = g.ct.get_or<max_durability>(selection_id, -1);
        lines.push_back(TextFormat("Durability: %d/%d", dura, max_dura));
    }
    else if (item_type == itemtype_t::SHIELD) {
        const int block = g.ct.get_or<block_chance>(selection_id, -1);
        lines.push_back(TextFormat("Block chance: %d", block));

        const int dura = g.ct.get_or<durability>(selection_id, -1);
        const int max_dura = g.ct.get_or<max_durability>(selection_id, -1);
        lines.push_back(TextFormat("Durability: %d/%d", dura, max_dura));
    }
    else if (item_type == itemtype_t::POTION) {
        const vec3 heal = g.ct.get_or<healing>(selection_id, vec3{-1, -1, -1});
        lines.push_back(TextFormat("Heal amount: %d-%d", heal.x, heal.y));
    }

    lines.push_back(g.ct.get_or<description>(selection_id, "no-description"));
    return lines;
}
