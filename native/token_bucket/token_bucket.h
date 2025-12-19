#ifndef TOKEN_BUCKET_H
#define TOKEN_BUCKET_H

#include <bits/time.h>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>
#include "clock.h"

using namespace std;

constexpr int64_t SCALE = 1000000;
constexpr int64_t CAPACITY = 100 * SCALE;
constexpr int64_t COST = 1 * SCALE;
constexpr int64_t REFILL_RATE_PER_SEC = 2 * SCALE;
constexpr int64_t MAX_ELAPSED_TIME = (CAPACITY * 1000000000LL) / REFILL_RATE_PER_SEC;

class TokenBucket {
private:
  Clock& clock;
  Clock::time_point prev_time;
  int64_t capacity;
  int64_t tokens;
  int64_t refill_rate_scaled; // this is also scaled to microseconds scaled ()
  int64_t cost_per_request_scaled;

  // chrono::time_point<chrono::steady_clock> prev_time;
  mutable mutex mtx;

public:
  TokenBucket(int64_t capacity,
              int64_t refill_rate,
              int64_t cost,
              Clock& clock);

  bool allow();

  int64_t tokens_remaining();
};

#endif // !TOKEN_BUCKET_H
