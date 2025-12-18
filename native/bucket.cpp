#include <algorithm>
#include <bits/time.h>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

// constants
constexpr int64_t SCALE = 1000000;
constexpr int64_t CAPACITY = 100 * SCALE;
constexpr int64_t COST = 1 * SCALE;
constexpr int64_t REFILL_RATE_PER_SEC = 2 * SCALE;

class TokenBucket {
private:
  int64_t capacity;
  int64_t tokens;
  int64_t refill_rate_scaled; // this is also scaled to microseconds scaled ()
  int64_t cost_per_request_scaled;

  chrono::time_point<std::chrono::steady_clock> prev_time;

public:
  TokenBucket(int64_t cap, int64_t refill_rate, int64_t cost)
      : capacity(cap), refill_rate_scaled(refill_rate),
        cost_per_request_scaled(cost) {
    tokens = 0;
    prev_time = std::chrono::steady_clock::now();
  }

  bool token_consumption() {
    // removing the while loop as it will be applied in the main when the user
    // access is made to this method

    chrono::time_point<std::chrono::steady_clock> t =
        std::chrono::steady_clock::now();
    auto elapsed_time =
        chrono::duration_cast<chrono::nanoseconds>(t - prev_time).count();

    prev_time = t;
    int64_t tokens_added = (elapsed_time * refill_rate_scaled) / 1000000000LL;

    tokens = min(tokens + tokens_added, capacity);

    cout << "Delta(t) " << elapsed_time << "ns"
         << ", tokens added: " << tokens_added << ", Total tokens: " << tokens
         << " " << "consumption allowed ? ";

    if (tokens >= cost_per_request_scaled) {
      tokens -= cost_per_request_scaled;
      return true;
    }

    return false;
  }

  // getter function to access the private state variables
  int64_t tokens_remaining() { return 0; };
};

int main(int argc, char *argv[]) {

  TokenBucket token_bucket(CAPACITY, REFILL_RATE_PER_SEC, COST);

  // because the loop will continue for ever its not limited to some limit
  // the system shall run forever through the course of the program

  while (true) {
    if (token_bucket.token_consumption()) {
      cout << "Success! Tokens remaining: " << token_bucket.tokens_remaining()
           << endl;
    } else {
      cout << "Not enough tokens" << endl;
    }
    this_thread::sleep_for(chrono::milliseconds(1000));
  }
  return 0;
}

// struct timespec tv;
// clock_gettime(CLOCK_MONOTONIC, &tv);
// (tv.tv_sec * 1000000000LL) + tv.tv_nsec;
// clock_gettime(CLOCK_MONOTONIC, &tv);
// long t = (tv.tv_sec * 1000000000LL) + tv.tv_nsec;
