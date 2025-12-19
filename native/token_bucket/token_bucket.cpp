#include "token_bucket.h"
#include "clock.h"
#include <algorithm>
#include <chrono>

using namespace std;

TokenBucket::TokenBucket(int64_t cap, int64_t refill_rate, int64_t cost, Clock& clock)
    : clock(clock), capacity(cap), refill_rate_scaled(refill_rate),
      cost_per_request_scaled(cost), tokens(0),
      prev_time(clock.now()) {}

bool TokenBucket::allow() {
  lock_guard<mutex> lock(mtx);

  chrono::time_point<std::chrono::steady_clock> t =
      clock.now();
  auto elapsed_time =
      chrono::duration_cast<chrono::nanoseconds>(t - prev_time).count();
  elapsed_time = min(elapsed_time, MAX_ELAPSED_TIME);
  prev_time = t;
  int64_t tokens_added = (elapsed_time * refill_rate_scaled) / 1000000000LL;

  tokens = min(tokens + tokens_added, capacity);

  // cout << "Delta(t) " << elapsed_time << " ns";

  if (tokens >= cost_per_request_scaled) {
    tokens -= cost_per_request_scaled;
    return true;
  }

  return false;
}

// getter function to access the private state variables
int64_t TokenBucket::tokens_remaining() {
  lock_guard<mutex> lock(mtx);
  return tokens;
};

/*
int main(int argc, char *argv[]) {

  TokenBucket token_bucket(CAPACITY, REFILL_RATE_PER_SEC, COST);
  // because the loop will continue for ever its not limited to some limit
  // the system shall run forever through the course of the program
  while (true) {
    if (token_bucket.allow()) {
      cout << "Success! Tokens remaining: " << token_bucket.tokens_remaining()
           << endl;
    } else {
      cout << "Not enough tokens" << endl;
    }
    this_thread::sleep_for(chrono::milliseconds(100));
  }
  return 0;
}
*/

// struct timespec tv;
// clock_gettime(CLOCK_MONOTONIC, &tv);
// (tv.tv_sec * 1000000000LL) + tv.tv_nsec;
// clock_gettime(CLOCK_MONOTONIC, &tv);
// long t = (tv.tv_sec * 1000000000LL) + tv.tv_nsec;
