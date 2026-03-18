#pragma once

#include "event_type.h"
#include <vector>

namespace rpg {

struct GameplayQueueState {
    bool processing_actions = false;
    std::vector<gameplay_event_t> gameplay_events;
};

} // namespace rpg
