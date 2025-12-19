#include "fake_clock.h"
#include "token_bucket.h"
#include <cassert>
#include <chrono>

void test_no_refill_without_time() {
  FakeClock clock;
  TokenBucket tb(CAPACITY, REFILL_RATE_PER_SEC, COST, clock);

  bool first = tb.allow();
  bool second = tb.allow();

  // first should fail (no tokens initially)
  assert(first == false);
  assert(second == false);
}

void test_refill_after_sleep() {
  FakeClock clock;
  TokenBucket tb(CAPACITY, REFILL_RATE_PER_SEC, COST, clock);

  clock.advance(std::chrono::seconds(1));

  bool allowed = tb.allow();
  assert(allowed == true);
}

void test_burst() {
  FakeClock clock;
  TokenBucket tb(CAPACITY, REFILL_RATE_PER_SEC, COST, clock);

  clock.advance(std::chrono::seconds(5));

  int success = 0;
  for (int i = 0; i < 5; i++) {
    if (tb.allow())
      success++;
  }

  assert(success == 5);
}

void test_capacity_cap() {
  FakeClock clock;
  TokenBucket tb(CAPACITY, REFILL_RATE_PER_SEC, COST, clock);

  clock.advance(std::chrono::seconds(200));

  assert(tb.tokens_remaining() <= CAPACITY);
}

int main() {
  test_no_refill_without_time();
  test_refill_after_sleep();
  test_burst();
  test_capacity_cap();
  return 0;
}
