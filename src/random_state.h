#pragma once

#include <chrono>
#include <random>
#include <string>
#include <raylib.h>

using std::mt19937;
using std::seed_seq;
using std::string;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;
using std::chrono::system_clock;
using std::chrono::time_point;

struct RandomState {
    mt19937 mt;
    string seed;

    void set_seed() {
        srand(time(NULL));
        SetRandomSeed(time(NULL));
        time_point<system_clock> now = system_clock::now();
        auto dur = duration_cast<nanoseconds>(now.time_since_epoch());
        long long nanoseconds_since_epoch = dur.count();
        seed = std::to_string(nanoseconds_since_epoch);
        seed_seq seedseq(seed.begin(), seed.end());
        mt.seed(seedseq);
    }
};
