#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>

struct Clock {
    using time_point = std::chrono::steady_clock::time_point;
    virtual ~Clock() = default;
    virtual time_point now() const = 0;
};

#endif
