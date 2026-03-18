#include "draw_message_history.h"

void draw_message_history(gamestate& g) {
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
            const string msg = g.messages.history.at(i);
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
        const string msg = g.messages.history.at(i);
        bzero(tmp, 1024);
        snprintf(tmp, sizeof(tmp), "%s", msg.c_str());
        const float msg_x = box.x + g.pad / 2.0;
        const float msg_y = box.y + g.pad / 2.0 + (i * (font_size + 2));
        DrawText(tmp, msg_x, msg_y, font_size, g.ui.window_box_fgcolor);
    }
}
