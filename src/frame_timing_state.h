#pragma once

#include <vector>

#ifndef LAST_FRAME_TIMES_MAX
#define LAST_FRAME_TIMES_MAX 1000
#endif

using std::vector;

struct FrameTimingState {
    unsigned int framecount = 0;
    unsigned int frame_updates = 0;
    unsigned long int ticks = 0;
    double last_frame_time = 0.0;
    double max_frame_time = 0.0;
    size_t last_frame_times_current = 0;
    vector<double> last_frame_times = vector<double>(LAST_FRAME_TIMES_MAX, 0.0);
    char frame_time_str[32] = {0};

    double get_avg_last_frame_time() const {
        if (last_frame_times.empty()) {
            return 0.0;
        }
        double sum = 0.0;
        for (double frame_time : last_frame_times) {
            sum += frame_time;
        }
        return sum / static_cast<double>(last_frame_times.size());
    }
};
