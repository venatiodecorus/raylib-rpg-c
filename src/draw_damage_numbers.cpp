#include "draw_damage_numbers.h"

#include <algorithm>
#include <raylib.h>

void draw_damage_numbers(gamestate& g) {
    if (g.damage_popups_sys.popups.empty()) {
        return;
    }

    for (const damage_popup_t& popup : g.damage_popups_sys.popups) {
        if (popup.floor != g.d.current_floor) {
            continue;
        }

        const float progress = popup.lifetime_seconds > 0.0f ? popup.age_seconds / popup.lifetime_seconds : 1.0f;
        const float clamped_progress = std::min(std::max(progress, 0.0f), 1.0f);
        const float rise = popup.rise_distance * (1.0f - (1.0f - clamped_progress) * (1.0f - clamped_progress));
        const float drift_x = popup.drift_x * clamped_progress;
        const float font_size = damage_popup_font_size_world(g, popup);
        const float spacing = 0.0f;
        const std::string text = std::to_string(popup.amount);
        const Vector2 measure = MeasureTextEx(GetFontDefault(), text.c_str(), font_size, spacing);
        const Vector2 pos = {
            popup.world_anchor.x + drift_x - measure.x / 2.0f,
            popup.world_anchor.y - rise,
        };
        const unsigned char alpha = static_cast<unsigned char>(255.0f * (1.0f - clamped_progress));
        const Color text_color = popup.critical ? Color{255, 64, 64, alpha} : Color{255, 255, 255, alpha};
        const Color shadow_color = Color{0, 0, 0, static_cast<unsigned char>(alpha * 0.7f)};

        DrawTextEx(GetFontDefault(), text.c_str(), Vector2{pos.x + 0.5f, pos.y + 0.5f}, font_size, spacing, shadow_color);
        DrawTextEx(GetFontDefault(), text.c_str(), pos, font_size, spacing, text_color);
    }
}
