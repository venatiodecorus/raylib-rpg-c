#pragma once

#include <ctime>
#include <string>

constexpr int GAMESTATE_SIZEOFTIMEBUF = 64;

namespace rpg {

struct RuntimeSessionState {
    time_t timebegan = 0;
    time_t currenttime = 0;
    struct tm* timebegantm = nullptr;
    struct tm* currenttimetm = nullptr;
    char timebeganbuf[GAMESTATE_SIZEOFTIMEBUF] = {0};
    char currenttimebuf[GAMESTATE_SIZEOFTIMEBUF] = {0};
    std::string version;
    unsigned int restart_count = 0;
};

} // namespace rpg
