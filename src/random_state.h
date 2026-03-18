#pragma once

#include <chrono>
#include <cstdlib>
#include <random>
#include <string>
#include <ctime>
#include <raylib.h>

namespace rpg {

struct RandomState {
    std::mt19937 mt;
    std::string seed;

    void set_seed() {
        std::srand(std::time(NULL));
        SetRandomSeed(std::time(NULL));
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
        long long nanoseconds_since_epoch = dur.count();
        seed = std::to_string(nanoseconds_since_epoch);
        std::seed_seq seedseq(seed.begin(), seed.end());
        mt.seed(seedseq);
    }
};

} // namespace rpg
