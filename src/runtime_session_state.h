#pragma once

#include <ctime>
#include <string>

#ifndef GAMESTATE_SIZEOFTIMEBUF
#define GAMESTATE_SIZEOFTIMEBUF 64
#endif

using std::string;

struct RuntimeSessionState {
    time_t timebegan = 0;
    time_t currenttime = 0;
    struct tm* timebegantm = nullptr;
    struct tm* currenttimetm = nullptr;
    char timebeganbuf[GAMESTATE_SIZEOFTIMEBUF] = {0};
    char currenttimebuf[GAMESTATE_SIZEOFTIMEBUF] = {0};
    string version;
    unsigned int restart_count = 0;
};
