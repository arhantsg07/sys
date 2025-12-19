#ifndef FAKE_CLOCK_H
#define FAKE_CLOCK_H

#include "clock.h"

struct FakeClock : public Clock {
    time_point current;

    FakeClock()
        : current(std::chrono::steady_clock::now()) {}

    time_point now() const override {
        return current;
    }

    void advance(std::chrono::nanoseconds ns) {
        current += ns;
    }
};

#endif