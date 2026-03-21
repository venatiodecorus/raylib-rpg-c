#include "draw_item_detail_text.h"

#include "ecs_gameplay_components.h"

vector<string> build_item_detail_lines(gamestate& g, entityid selection_id) {
    vector<string> lines;
    const itemtype_t item_type = g.get_component_or<ItemSubtype>(selection_id, itemtype_t::NONE);

    lines.push_back(g.get_component_or<EntityName>(selection_id, std::string{"no-name"}));

    if (item_type == itemtype_t::WEAPON) {
        const vec3 dmg = g.get_component_or<DamageDice>(selection_id, vec3{-1, -1, -1});
        lines.push_back(TextFormat("Damage: %d-%d", dmg.x, dmg.y));

        const int dura = g.get_component_or<ItemDurability>(selection_id, -1);
        const int max_dura = g.get_component_or<ItemMaxDurability>(selection_id, -1);
        lines.push_back(TextFormat("Durability: %d/%d", dura, max_dura));
    }
    else if (item_type == itemtype_t::SHIELD) {
        const int block = g.get_component_or<BlockChance>(selection_id, -1);
        lines.push_back(TextFormat("Block chance: %d", block));

        const int dura = g.get_component_or<ItemDurability>(selection_id, -1);
        const int max_dura = g.get_component_or<ItemMaxDurability>(selection_id, -1);
        lines.push_back(TextFormat("Durability: %d/%d", dura, max_dura));
    }
    else if (item_type == itemtype_t::POTION) {
        const vec3 heal = g.get_component_or<HealingDice>(selection_id, vec3{-1, -1, -1});
        lines.push_back(TextFormat("Heal amount: %d-%d", heal.x, heal.y));
    }

    lines.push_back(g.get_component_or<EntityDescription>(selection_id, std::string{"no-description"}));
    return lines;
}
