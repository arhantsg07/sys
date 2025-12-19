#ifndef STEADY_CLOCK_H
#define STEADY_CLOCK_H

#include "clock.h"

struct SteadyClock : public Clock {
    time_point now() const override {
        return std::chrono::steady_clock::now();
    }
};

#endif