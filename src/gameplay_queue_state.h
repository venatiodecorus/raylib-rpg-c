#pragma once

#include "event_type.h"
#include <vector>

using std::vector;

struct GameplayQueueState {
    bool processing_actions = false;
    vector<gameplay_event_t> gameplay_events;
};
