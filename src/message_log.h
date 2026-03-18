/** @file message_log.h
 *  @brief Message queue and history log subsystem.
 */
#pragma once

#include <raylib.h>
#include <string>
#include <vector>

class MessageLog {
public:
    std::vector<std::string> system;      // active on-screen messages
    std::vector<std::string> history;     // persistent log
    bool is_active = false;              // messages pending display
    unsigned int max_len_msg = 0;        // longest message char count
    unsigned int max_len_msg_measure = 0; // pixel width of longest

    bool add(const char* fmt, ...);
    void add_history(const char* fmt, ...);
    void cycle();
};
