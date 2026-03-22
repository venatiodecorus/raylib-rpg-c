/** @file hud_renderer.cpp
 *  @brief HudRenderer implementation — player HUD, message history, message box, interaction modal, look panel, level-up modal.
 */

#include "hud_renderer.h"

#include "ecs_gameplay_components.h"

#include <array>

namespace rpg
{

void HudRenderer::draw_hud(gamestate& g) {
    constexpr int font_size = DEFAULT_HUD_FONT_SIZE;
    constexpr int line_thickness = 2;
    constexpr int line_gap = 2;
    constexpr int line_count = 5;
    constexpr int inner_pad = 8;

    const int turn = g.turn_count;
    const vec2 hp_value = g.get_component_or<HitPoints>(g.hero_id, vec2{-1, -1});
    const int myhp = hp_value.x;
    const int mymaxhp = hp_value.y;
    const int mylevel = g.get_component_or<EntityLevel>(g.hero_id, 0);
    const int myxp = g.get_component_or<Experience>(g.hero_id, 0);
    const int attack_bonus = get_stat_bonus(g.get_component_or<StrengthAttr>(g.hero_id, 10));
    const int ac = g.compute_armor_class(g.hero_id);
    const int str = g.get_component_or<StrengthAttr>(g.hero_id, -1);
    const int dex = g.get_component_or<DexterityAttr>(g.hero_id, -1);
    const int con = g.get_component_or<ConstitutionAttr>(g.hero_id, -1);
    const int int_ = g.get_component_or<IntelligenceAttr>(g.hero_id, -1);
    const int wis = g.get_component_or<WisdomAttr>(g.hero_id, -1);
    const int cha = g.get_component_or<CharismaAttr>(g.hero_id, -1);
    const string n = g.get_component_or<EntityName>(g.hero_id, std::string{"no-name"});
    const vec3 loc = g.get_component_or<Position>(g.hero_id, vec3{-1, -1, -1});
    const alignment_t hero_alignment = g.get_component_or<AlignmentComponent>(g.hero_id, alignment_t::NONE);
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

void HudRenderer::draw_look_panel(gamestate& g) {
    auto loc = g.get_component_or<Position>(g.hero_id, vec3{-1, -1, -1});
    auto df = g.d.get_floor(loc.z);
    tile_t& tile = df->tile_at(loc);
    const int entity_count = tile.entity_count() - 1;
    const int texts_size = 7;
    const char* dummy_text = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    const int font_size = DEFAULT_LOOK_PANEL_FONT_SIZE;
    const int pad_w = 10;
    const int pad_h = 10;
    const int m = MeasureText(dummy_text, font_size);
    const float w = m + pad_w * 2;
    const float h = font_size * texts_size + pad_h * (texts_size + 2);
    const float base_x = 10;
    const float base_y = g.targetheight - h;
    Rectangle r = {base_x, base_y, w, h};

    DrawRectanglePro(r, Vector2{0, 0}, 0.0f, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(r, 1, g.ui.window_box_fgcolor);
    DrawText("Look Panel", r.x + pad_w, r.y + pad_h + (font_size + 5) * 0, font_size, g.ui.window_box_fgcolor);
    DrawText(TextFormat("There are %d things here", entity_count), r.x + pad_w, r.y + pad_h + (font_size + 5) * 1, font_size, g.ui.window_box_fgcolor);
    DrawText(TextFormat("living npc %d", tile.get_cached_live_npc()), r.x + pad_w, r.y + pad_h + (font_size + 5) * 2, font_size, g.ui.window_box_fgcolor);
    DrawText(TextFormat("dead npc %d", tile.get_cached_dead_npc()), r.x + pad_w, r.y + pad_h + (font_size + 5) * 3, font_size, g.ui.window_box_fgcolor);
    DrawText(
        TextFormat("items %d (%d)", static_cast<int>(tile.get_item_count()), tile.get_cached_item()),
        r.x + pad_w,
        r.y + pad_h + (font_size + 5) * 4,
        font_size,
        g.ui.window_box_fgcolor);
    DrawText(TextFormat("door %d", tile.get_cached_door()), r.x + pad_w, r.y + pad_h + (font_size + 5) * 5, font_size, g.ui.window_box_fgcolor);
    DrawText(TextFormat("box %d", tile.get_cached_box()), r.x + pad_w, r.y + pad_h + (font_size + 5) * 6, font_size, g.ui.window_box_fgcolor);
}

void HudRenderer::draw_message_history(gamestate& g) {
    char tmp[1024] = {0};
    const int font_size = DEFAULT_MSG_HISTORY_FONT_SIZE;
    constexpr int max_messages = 30;
    const int msg_count = g.messages.history.size();
    if (msg_count == 0) {
        return;
    }
    const int max_measure = g.messages.max_len_msg_measure;

    const float w = max_measure + g.pad;
    const float h = (font_size + 2) * std::min(msg_count, max_messages) + g.pad;
    const float x = g.targetwidth - w;
    constexpr float y = 0;
    const Rectangle box = {x, y, w, h};
    DrawRectangleRec(box, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(box, 1, g.ui.window_box_fgcolor);

    if (msg_count > max_messages) {
        int outer_count = 0;
        for (int i = msg_count - max_messages; i < msg_count; i++) {
            const string& msg = g.messages.history.at(i);
            bzero(tmp, 1024);
            snprintf(tmp, sizeof(tmp), "%s", msg.c_str());
            const float msg_x = box.x + g.pad / 2.0;
            const float msg_y = box.y + g.pad / 2.0 + (outer_count * (font_size + 2));
            DrawText(tmp, msg_x, msg_y, font_size, g.ui.window_box_fgcolor);
            outer_count++;
        }
        return;
    }
    for (int i = 0; i < msg_count; i++) {
        const string& msg = g.messages.history.at(i);
        bzero(tmp, 1024);
        snprintf(tmp, sizeof(tmp), "%s", msg.c_str());
        const float msg_x = box.x + g.pad / 2.0;
        const float msg_y = box.y + g.pad / 2.0 + (i * (font_size + 2));
        DrawText(tmp, msg_x, msg_y, font_size, g.ui.window_box_fgcolor);
    }
}

void HudRenderer::draw_message_box(gamestate& g) {
    if (!g.ui.display_confirm_prompt && (!g.messages.is_active || g.messages.system.size() == 0)) {
        return;
    }

    constexpr int w = DEFAULT_TARGET_WIDTH;
    constexpr int font_size = DEFAULT_MSG_WINDOW_FONT_SIZE;
    constexpr float text_height = font_size;
    constexpr float y = (DEFAULT_TARGET_HEIGHT - text_height) / 2.0 - DEFAULT_PAD;

    const string& msg = g.ui.display_confirm_prompt ? g.ui.confirm_prompt_message : g.messages.system.at(0);
    char tmp[1024] = {0};
    snprintf(tmp, sizeof(tmp), "%s", msg.c_str());

    const int measure = MeasureText(tmp, font_size);
    const float text_width = measure;
    const float x = (w - text_width) / 2.0 - DEFAULT_PAD;
    const float box_width = text_width + DEFAULT_PAD * 2;
    const float box_height = text_height + g.pad * 2;
    const Rectangle box = {x, y, box_width, box_height};

    DrawRectangleRec(box, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(box, 1, g.ui.window_box_fgcolor);

    const int text_x = box.x + g.pad;
    const int text_y = box.y + g.pad;
    DrawText(tmp, text_x, text_y, font_size, g.ui.window_box_fgcolor);
}

void HudRenderer::draw_interaction_modal(gamestate& g) {
    if (!g.ui.display_interaction_modal || g.ui.interaction_body.empty()) {
        return;
    }

    constexpr int title_font_size = 24;
    constexpr int body_font_size = DEFAULT_MSG_WINDOW_FONT_SIZE;
    constexpr int line_spacing = 6;
    constexpr int padding = DEFAULT_PAD;
    constexpr int min_width = 360;
    vector<string> lines;
    if (!g.ui.interaction_title.empty()) {
        lines.push_back(g.ui.interaction_title);
    }

    size_t line_start = 0;
    while (line_start <= g.ui.interaction_body.size()) {
        const size_t line_end = g.ui.interaction_body.find('\n', line_start);
        const string line =
            line_end == string::npos ? g.ui.interaction_body.substr(line_start) : g.ui.interaction_body.substr(line_start, line_end - line_start);
        lines.push_back(line);
        if (line_end == string::npos) {
            break;
        }
        line_start = line_end + 1;
    }

    int max_text_width = 0;
    for (size_t i = 0; i < lines.size(); i++) {
        const int font_size = i == 0 && !g.ui.interaction_title.empty() ? title_font_size : body_font_size;
        max_text_width = std::max(max_text_width, MeasureText(lines[i].c_str(), font_size));
    }

    const int box_width = std::max(min_width, max_text_width + padding * 2);
    const int title_height = g.ui.interaction_title.empty() ? 0 : title_font_size;
    const int body_lines = static_cast<int>(lines.size()) - static_cast<int>(!g.ui.interaction_title.empty());
    const int body_height = body_lines > 0 ? body_lines * body_font_size + (body_lines - 1) * line_spacing : 0;
    const int section_gap = (!g.ui.interaction_title.empty() && body_lines > 0) ? line_spacing + 4 : 0;
    const int box_height = padding * 2 + title_height + body_height + section_gap;
    const int box_x = (DEFAULT_TARGET_WIDTH - box_width) / 2;
    const int box_y = (DEFAULT_TARGET_HEIGHT - box_height) / 2;

    DrawRectangle(box_x, box_y, box_width, box_height, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(
        Rectangle{static_cast<float>(box_x), static_cast<float>(box_y), static_cast<float>(box_width), static_cast<float>(box_height)},
        2,
        g.ui.window_box_fgcolor);

    int text_y = box_y + padding;
    if (!g.ui.interaction_title.empty()) {
        const int title_width = MeasureText(g.ui.interaction_title.c_str(), title_font_size);
        const int title_x = box_x + (box_width - title_width) / 2;
        DrawText(g.ui.interaction_title.c_str(), title_x, text_y, title_font_size, g.ui.window_box_fgcolor);
        text_y += title_font_size + section_gap;
    }

    const int body_x = box_x + padding;
    for (size_t i = g.ui.interaction_title.empty() ? 0 : 1; i < lines.size(); i++) {
        DrawText(lines[i].c_str(), body_x, text_y, body_font_size, g.ui.window_box_fgcolor);
        text_y += body_font_size + line_spacing;
    }
}

void HudRenderer::draw_level_up_modal(gamestate& g) {
    if (!g.ui.display_level_up_modal) {
        return;
    }

    constexpr int title_font_size = 26;
    constexpr int body_font_size = 18;
    constexpr int cell_font_size = 20;
    constexpr int padding = DEFAULT_PAD;
    constexpr int cols = 2;
    constexpr int rows = 3;
    constexpr int cell_w = 200;
    constexpr int cell_h = 52;
    constexpr int cell_gap = 12;

    const std::array<const char*, 6> labels = {
        "Strength",
        "Dexterity",
        "Constitution",
        "Intelligence",
        "Wisdom",
        "Charisma",
    };

    const int grid_w = cols * cell_w + (cols - 1) * cell_gap;
    const int grid_h = rows * cell_h + (rows - 1) * cell_gap;
    const int box_w = grid_w + padding * 2;
    const int box_h = padding * 2 + title_font_size + body_font_size + 20 + grid_h;
    const int box_x = (DEFAULT_TARGET_WIDTH - box_w) / 2;
    const int box_y = (DEFAULT_TARGET_HEIGHT - box_h) / 2;

    DrawRectangle(box_x, box_y, box_w, box_h, g.ui.window_box_bgcolor);
    DrawRectangleLinesEx(
        Rectangle{static_cast<float>(box_x), static_cast<float>(box_y), static_cast<float>(box_w), static_cast<float>(box_h)}, 2, g.ui.window_box_fgcolor);

    const char* title = "Level Up";
    const char* subtitle = "Choose one attribute to permanently increase by 1";
    const int title_x = box_x + (box_w - MeasureText(title, title_font_size)) / 2;
    const int subtitle_x = box_x + (box_w - MeasureText(subtitle, body_font_size)) / 2;
    DrawText(title, title_x, box_y + padding, title_font_size, g.ui.window_box_fgcolor);
    DrawText(subtitle, subtitle_x, box_y + padding + title_font_size + 8, body_font_size, g.ui.window_box_fgcolor);

    const int grid_x = box_x + padding;
    const int grid_y = box_y + padding + title_font_size + body_font_size + 20;
    for (int i = 0; i < 6; i++) {
        const int col = i % cols;
        const int row = i / cols;
        const int cell_x = grid_x + col * (cell_w + cell_gap);
        const int cell_y = grid_y + row * (cell_h + cell_gap);
        const bool selected = static_cast<int>(g.ui.level_up_selection % 6) == i;
        const Color fill = selected ? Color{0x00, 0x88, 0x44, 220} : Color{0x11, 0x11, 0x66, 180};
        DrawRectangle(cell_x, cell_y, cell_w, cell_h, fill);
        DrawRectangleLinesEx(
            Rectangle{static_cast<float>(cell_x), static_cast<float>(cell_y), static_cast<float>(cell_w), static_cast<float>(cell_h)},
            2,
            g.ui.window_box_fgcolor);

        const int text_w = MeasureText(labels[i], cell_font_size);
        const int text_x = cell_x + (cell_w - text_w) / 2;
        const int text_y = cell_y + (cell_h - cell_font_size) / 2;
        DrawText(labels[i], text_x, text_y, cell_font_size, g.ui.window_box_fgcolor);
    }
}

} // namespace rpg
