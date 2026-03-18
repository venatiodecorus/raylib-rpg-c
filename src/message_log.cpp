/** @file message_log.cpp
 *  @brief Message queue and history log subsystem implementation.
 */

#include "message_log.h"
#include "libgame_defines.h"
#include <cstdarg>
#include <cstdio>
#include <cassert>

namespace rpg {

bool MessageLog::add(const char* fmt, ...) {
    assert(fmt && "format string is NULL");
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer) - 1, fmt, args);
    va_end(args);
    system.push_back(std::string(buffer));
    is_active = true;
    return true;
}

void MessageLog::add_history(const char* fmt, ...) {
    assert(fmt && "format string is NULL");
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer) - 1, fmt, args);
    va_end(args);
    history.push_back(std::string(buffer));
}

void MessageLog::cycle() {
    if (system.empty()) {
        is_active = false;
        return;
    }
    std::string msg = system.front();
    unsigned int len = msg.length();
    if (len > max_len_msg) {
        max_len_msg = len;
        constexpr int font_size = DEFAULT_MSG_HISTORY_FONT_SIZE;
        int measure = MeasureText(msg.c_str(), font_size);
        max_len_msg_measure = measure;
    }
    history.push_back(msg);
    system.erase(system.begin());
    if (system.empty()) {
        is_active = false;
    }
}

} // namespace rpg
