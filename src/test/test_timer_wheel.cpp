#include "timerEvent.h"
#include "timerWheel.h"
#include <cassert>
#include <cstdio>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

void timer_node_print(_timer::time_t exp) { cout << exp << endl; };

void timeCore_add_print_timerNode(_timer::time_t exp) {
  auto node = _timerEvent::New(1, exp, timer_node_print, exp);
  _timer::timerWheel::getInstance().add_timer_node(*node);
};

int main() {
  _timer::List timeouts;
  auto &timer_core = _timer::timerWheel::getInstance();
  auto now = timer_core.update_time(0, timeouts);

  assert(timeouts.empty());

  std::vector<_timer::time_t> exps{
      0,
      999,
      (1UL << 10) - 1,
      (1UL << 10),
      (1UL << 10) + 1000,
      ((1UL << 10) << 1) - 1,
      (1UL << 10) << 1,
      (1UL << 20),
      (1UL << 30),
      (1UL << 40),
      (1UL << 50) - 1,
      (1UL << 50),
      (1UL << 50) + 100,
  };
  for (auto &e : exps) {
    timeCore_add_print_timerNode(e);
  }

  for (int i = 0; i < 7; ++i) {
    auto wheel = timer_core.wheels[i];
    if (wheel) {
      cout << "Wheel-level : " << i << endl;
      for (int j = 0; j < TIMER_SLOT_SIZE; ++j) {
        auto &slot = wheel->slot[j];
        if (!slot.empty()) {
          cout << "\t" << j << " : \t" << slot.size() << endl;
        }
      }
    }
  }

  now = timer_core.update_time(2, timeouts);
  cout << timeouts.size() << " | " << now << endl;

  now = timer_core.update_time(1024, timeouts);
  cout << timeouts.size() << " | " << now << endl;

  now = timer_core.update_time(1048576, timeouts);
  cout << timeouts.size() << " | " << now << endl;
};