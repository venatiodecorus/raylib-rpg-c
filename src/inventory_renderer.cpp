/** @file inventory_renderer.cpp
 *  @brief InventoryRenderer implementation — inventory, chest, mini-inventory, item detail text.
 */

#include "inventory_renderer.h"

#include "ecs_gameplay_components.h"

namespace rpg
{

vector<string> InventoryRenderer::build_item_detail_lines(gamestate& g, entityid selection_id) {
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

void InventoryRenderer::draw_item_detail_panel(gamestate& g, rpg::Renderer& renderer, const Rectangle& right_box, entityid selection_id) {
    auto it = renderer.spritegroups.find(selection_id);
    spritegroup* sg = it != renderer.spritegroups.end() ? it->second.get() : nullptr;
    if (!sg) {
        return;
    }

    auto sprite = sg->get_current();
    DrawTexturePro(*(sprite->get_texture()), Rectangle{0, 0, 32, 32}, right_box, Vector2{0, 0}, 0.0f, WHITE);

    const int fontsize = 20;
    const int cur_x = right_box.x + 10;
    int cur_y = right_box.y + 10;
    const int y_incr = 20;
    const vector<string> lines = build_item_detail_lines(g, selection_id);
    for (const string& line : lines) {
        DrawText(line.c_str(), cur_x, cur_y, fontsize, g.ui.window_box_fgcolor);
        cur_y += y_incr;
    }
}

void InventoryRenderer::draw_inventory_grid(
    gamestate& g,
    rpg::Renderer& renderer,
    const vector<entityid>& inventory,
    const Rectangle& left_box,
    bool show_equipped_labels) {
    float x = left_box.x + 2;
    float y = left_box.y + 2;
    const int cols = 7;
    const int rows = 7;
    const float w = (left_box.width - 4) / cols;
    const float h = (left_box.height - 4) / rows;
    auto it = inventory.begin();
    for (int j = 0; j < rows; j++) {
        for (int i = 0; i < cols; i++) {
            Rectangle grid_box = {x, y, w, h};
            Rectangle grid_box2 = {x + 2, y + 2, w - 4, h - 4};
            DrawRectangleLinesEx(grid_box, 1, Color{0x66, 0x66, 0x66, 255});
            if (it != inventory.end()) {
                auto sg_it = renderer.spritegroups.find(*it);
                spritegroup* sg = sg_it != renderer.spritegroups.end() ? sg_it->second.get() : nullptr;
                if (sg) {
                    auto sprite = sg->get_current();
                    DrawTexturePro(*(sprite->get_texture()), Rectangle{10, 10, 12, 12}, grid_box2, Vector2{0, 0}, 0.0f, WHITE);
                    if (show_equipped_labels) {
                        size_t index = static_cast<size_t>(j * cols + i);
                        if (index < inventory.size()) {
                            const entityid selection_id = inventory.at(index);
                            const entityid cur_wpn_id = g.get_component_or<EquippedWeapon>(g.hero_id, ENTITYID_INVALID);
                            const entityid cur_shield_id = g.get_component_or<EquippedShield>(g.hero_id, ENTITYID_INVALID);
                            const bool cur_wpn_selected = selection_id == cur_wpn_id && cur_wpn_id != ENTITYID_INVALID;
                            const bool cur_shield_selected = selection_id == cur_shield_id && cur_shield_id != ENTITYID_INVALID;
                            if (cur_wpn_selected || cur_shield_selected) {
                                DrawText("equipped", grid_box2.x, grid_box2.y + grid_box2.height - 10, 10, g.ui.window_box_fgcolor);
                            }
                        }
                    }
                }
                ++it;
            }
            if (static_cast<float>(i) == g.ui.inventory_cursor.x && static_cast<float>(j) == g.ui.inventory_cursor.y) {
                DrawRectangleLinesEx(grid_box, 2, GREEN);
            }
            x += w;
        }
        x = left_box.x + 2;
        y += h;
    }
}

void InventoryRenderer::draw_chest(gamestate& g, rpg::Renderer& renderer) {
    if (!g.ui.display_chest_menu || g.active_chest_id == ENTITYID_INVALID) {
        return;
    }

    const Inventory* inventory_ptr = g.get_component<Inventory>(g.ui.chest_deposit_mode ? g.hero_id : g.active_chest_id);
    if (!inventory_ptr) {
        return;
    }

    const auto& inventory = inventory_ptr->value;
    const char* menu_title = g.ui.chest_deposit_mode ? "Treasure Chest - Deposit" : "Treasure Chest";
    const char* hint_text = g.ui.chest_deposit_mode ? "TAB: chest view  ENTER: deposit  ESC/D: close" : "TAB: hero inventory  ENTER: take  ESC/D: close";
    const int section_gap = 8;
    const int font_size = 10;
    const Vector2 title_size = MeasureTextEx(GetFontDefault(), menu_title, font_size, g.line_spacing);
    const int w = DEFAULT_TARGET_WIDTH;
    const int h = DEFAULT_TARGET_HEIGHT;
    const float menu_width = w * 0.75f;
    const float menu_height = h * 0.75f;
    Rectangle menu_box = {(w - menu_width) / 2.0f, (h - menu_height) / 4.0f, menu_width, menu_height};
    float title_x = menu_box.x + (menu_box.width - title_size.x) / 2.0f;
    float title_y = menu_box.y + g.pad;
    float half_width = (menu_box.width - section_gap) / 2.0f;
    float half_height = menu_box.height - title_size.y - g.pad * 3.0f;

    DrawRectangleRec(menu_box, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(menu_box, 2, g.ui.window_box_fgcolor);
    DrawText(menu_title, title_x, title_y, font_size, g.ui.window_box_fgcolor);
    DrawText(hint_text, menu_box.x + g.pad, menu_box.y + menu_box.height - g.pad, font_size, g.ui.window_box_fgcolor);

    Rectangle left_box = {menu_box.x + g.pad, title_y + title_size.y + g.pad, half_width - g.pad, half_height};
    Rectangle right_box = {left_box.x + half_width + section_gap, left_box.y, half_width - g.pad * 2, half_height};
    DrawRectangleRec(left_box, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(left_box, 2, g.ui.window_box_fgcolor);
    DrawRectangleRec(right_box, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(right_box, 2, g.ui.window_box_fgcolor);

    draw_inventory_grid(g, renderer, inventory, left_box, g.ui.chest_deposit_mode);

    if (!inventory.empty()) {
        size_t index = static_cast<size_t>(g.ui.inventory_cursor.y) * 7 + static_cast<size_t>(g.ui.inventory_cursor.x);
        if (index < inventory.size()) {
            draw_item_detail_panel(g, renderer, right_box, inventory.at(index));
        }
    }
}

void InventoryRenderer::draw_inventory(gamestate& g, rpg::Renderer& renderer) {
    if (!g.ui.display_inventory_menu) {
        return;
    }

    const char* menu_title = "Inventory Menu";
    const int section_gap = 8;
    const int font_size = 10;
    const Vector2 title_size = MeasureTextEx(GetFontDefault(), menu_title, font_size, g.line_spacing);
    const float menu_width_percent = 0.75f;
    const float menu_height_percent = 0.75f;
    const int w = DEFAULT_TARGET_WIDTH;
    const int h = DEFAULT_TARGET_HEIGHT;
    const float menu_width = w * menu_width_percent;
    const float menu_height = h * menu_height_percent;
    Rectangle menu_box = {(w - menu_width) / 2.0f, (h - menu_height) / 4.0f, menu_width, menu_height};
    float title_x = menu_box.x + (menu_box.width - title_size.x) / 2.0f;
    float title_y = menu_box.y + g.pad;
    float half_width = (menu_box.width - section_gap) / 2.0f;
    float half_height = menu_box.height - title_size.y - g.pad * 3.0f;

    DrawRectangleRec(menu_box, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(menu_box, 2, g.ui.window_box_fgcolor);
    DrawText(menu_title, title_x, title_y, font_size, g.ui.window_box_fgcolor);

    Rectangle left_box = {menu_box.x + g.pad, title_y + title_size.y + g.pad, half_width - g.pad, half_height};
    Rectangle right_box = {left_box.x + half_width + section_gap, left_box.y, half_width - g.pad * 2, half_height};
    DrawRectangleRec(left_box, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(left_box, 2, g.ui.window_box_fgcolor);
    DrawRectangleRec(right_box, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(right_box, 2, g.ui.window_box_fgcolor);

    const Inventory* inv = g.get_component<Inventory>(g.hero_id);
    if (inv == nullptr) {
        return;
    }
    draw_inventory_grid(g, renderer, inv->value, left_box, true);

    if (!inv->value.empty()) {
        size_t index = static_cast<size_t>(g.ui.inventory_cursor.y) * 7 + static_cast<size_t>(g.ui.inventory_cursor.x);
        if (index < inv->value.size()) {
            draw_item_detail_panel(g, renderer, right_box, inv->value.at(index));
        }
    }
}

Rectangle InventoryRenderer::panel_for_hero(gamestate& g, float width, float height) {
    const vec3 hero_loc = g.get_component_or<Position>(g.hero_id, vec3{0, 0, 0});
    const Vector2 hero_screen = GetWorldToScreen2D(
        Vector2{
            static_cast<float>(hero_loc.x * DEFAULT_TILE_SIZE) + DEFAULT_TILE_SIZE * 0.5f,
            static_cast<float>(hero_loc.y * DEFAULT_TILE_SIZE),
        },
        g.cam2d);
    const float margin = 12.0f;
    const float x = std::clamp(hero_screen.x + 36.0f, margin, g.targetwidth - width - margin);
    const float y = std::clamp(hero_screen.y - height * 0.5f, margin, g.targetheight - height - margin);
    return Rectangle{x, y, width, height};
}

void InventoryRenderer::draw_mini_inventory(
    gamestate& g,
    rpg::Renderer& renderer,
    const vector<entityid>& inventory,
    const char* title,
    const char* hint,
    bool show_equipped) {
    const float width = 260.0f;
    const float row_h = 18.0f;
    const float preview_h = 118.0f;
    const float padding = 10.0f;
    const float footer_h = 18.0f;
    const size_t visible_count = std::max(1U, g.ui.mini_inventory_visible_count);
    const float list_h = row_h * static_cast<float>(visible_count);
    const Rectangle panel = panel_for_hero(g, width, padding * 3.0f + 14.0f + list_h + preview_h + footer_h + 12.0f);
    DrawRectangleRec(panel, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(panel, 2, g.ui.window_box_fgcolor);
    DrawText(title, static_cast<int>(panel.x + padding), static_cast<int>(panel.y + padding), 12, g.ui.window_box_fgcolor);

    const float hint_y = panel.y + panel.height - footer_h;
    DrawText(hint, static_cast<int>(panel.x + padding), static_cast<int>(hint_y), 10, g.ui.window_box_fgcolor);

    const Rectangle list_box = {panel.x + padding, panel.y + 28.0f, panel.width - padding * 2.0f, list_h};
    DrawRectangleLinesEx(list_box, 1, g.ui.window_box_fgcolor);

    const size_t selected_index = g.get_inventory_selection_index();
    const size_t scroll = g.ui.mini_inventory_scroll_offset;
    for (size_t i = 0; i < visible_count; i++) {
        const float row_y = list_box.y + row_h * static_cast<float>(i);
        const Rectangle row = {list_box.x, row_y, list_box.width, row_h};
        const size_t item_index = scroll + i;
        if (item_index == selected_index && item_index < inventory.size()) {
            DrawRectangleRec(row, Color{255, 255, 255, 28});
            DrawRectangleLinesEx(row, 1, YELLOW);
        }
        if (item_index >= inventory.size()) {
            continue;
        }

        const entityid item_id = inventory.at(item_index);
        const string item_name = g.get_component_or<EntityName>(item_id, std::string{"no-name"});
        const bool equipped = show_equipped && (item_id == g.get_component_or<EquippedWeapon>(g.hero_id, ENTITYID_INVALID) ||
                                                item_id == g.get_component_or<EquippedShield>(g.hero_id, ENTITYID_INVALID));
        DrawText(
            TextFormat("%s%s", item_index == selected_index ? "> " : "  ", item_name.c_str()),
            static_cast<int>(row.x + 6),
            static_cast<int>(row.y + 4),
            10,
            item_index == selected_index ? YELLOW : g.ui.window_box_fgcolor);
        if (equipped) {
            DrawText("E", static_cast<int>(row.x + row.width - 14), static_cast<int>(row.y + 4), 10, g.ui.window_box_fgcolor);
        }
    }

    if (scroll > 0) {
        DrawText("^", static_cast<int>(list_box.x + list_box.width - 12), static_cast<int>(list_box.y - 10), 10, g.ui.window_box_fgcolor);
    }
    if (scroll + visible_count < inventory.size()) {
        DrawText("v", static_cast<int>(list_box.x + list_box.width - 12), static_cast<int>(list_box.y + list_box.height), 10, g.ui.window_box_fgcolor);
    }

    if (inventory.empty() || selected_index >= inventory.size()) {
        DrawText("empty", static_cast<int>(panel.x + padding), static_cast<int>(list_box.y + list_box.height + 10), 10, g.ui.window_box_fgcolor);
        return;
    }

    const entityid selection_id = inventory.at(selected_index);
    const Rectangle preview = {panel.x + padding, list_box.y + list_box.height + 10.0f, panel.width - padding * 2.0f, preview_h};
    DrawRectangleLinesEx(preview, 1, g.ui.window_box_fgcolor);
    auto it = renderer.spritegroups.find(selection_id);
    spritegroup* sg = it != renderer.spritegroups.end() ? it->second.get() : nullptr;
    const float preview_sprite_size = 72.0f;
    if (sg) {
        auto sprite = sg->get_current();
        DrawTexturePro(
            *(sprite->get_texture()),
            Rectangle{0, 0, 32, 32},
            Rectangle{preview.x + 8, preview.y + 8, preview_sprite_size, preview_sprite_size},
            Vector2{0, 0},
            0.0f,
            WHITE);
    }
    const vector<string> lines = build_item_detail_lines(g, selection_id);
    const float text_x = preview.x + preview_sprite_size + 18.0f;
    float text_y = preview.y + 8.0f;
    for (size_t i = 0; i < lines.size() && i < 3; i++) {
        DrawText(lines[i].c_str(), static_cast<int>(text_x), static_cast<int>(text_y), i == 0 ? 11 : 10, g.ui.window_box_fgcolor);
        text_y += 20.0f;
    }
    if (!lines.empty()) {
        DrawText(lines.back().c_str(), static_cast<int>(preview.x + 8), static_cast<int>(preview.y + preview_sprite_size + 14.0f), 10, g.ui.window_box_fgcolor);
    }
}

} // namespace rpg
