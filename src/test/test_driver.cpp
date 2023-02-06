#include "timerLogger.h"
#include "timerDriver.h"
#include "timerEvent.h"
#include <bits/stdint-uintn.h>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <unistd.h>
#include <vector>

using std::cout;
using std::endl;

static std::vector<
    std::chrono::duration<long, struct std::ratio<1, 1000000000>>::rep>
    tms;
static std::vector<unsigned long int> timer_ids;

void timer_node_print(_timer::time_t exp) {
  // cout << exp << "\t" << endl;
  auto now = std::chrono::system_clock::now().time_since_epoch().count();
  LOG::trace("timer_node_print  {} \t {}", exp, now);
  tms.emplace_back(now);
};

void timeCore_add_print_timerNode(_timer::time_t exp) {
  auto node = _timerEvent::New(exp == 999 ? 3 : 1, exp, timer_node_print, exp);
  _timer::timerWheel::get_instance().add_timer_node(*node);
  timer_ids.emplace_back(node->timerID());
};

int main() {
  _timer::List timeouts;
  LOG::init("logs/test_dirve.log");
  std::vector<_timer::time_t> exps{
      // 0,
      1,
      500,
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

  // cout << std::chrono::system_clock::now().time_since_epoch().count() <<
  // endl;
  // tms.push_back(std::chrono::system_clock::now().time_since_epoch().count());
  // usleep(100);
  _timer::timerDriver::get_instance();
  usleep(1000 * 1000 * 30);
  for (uint64_t i = 0; i < tms.size(); ++i) {
    cout << exps[i] << endl;
  }
  // for (uint64_t i = 1; i < tms.size(); ++i) {
  //   // auto &&t = tms[i];
  //   cout << exps[i] - exps[i - 1] << " | "
  //        << float(tms[i] - tms[i - 1]) / 1000 / TICK_INTERVAL_US << endl;
  // }

  // LOG::reset();

  cout << "timer event using-size = "
       << _timerEvent::timerEventObject::useing.size()
       << ",\t expired-size = " << _timerEvent::timerEventObject::expired.size()
       << endl;

  for (auto &&id : timer_ids) {
    cout << id << ", ";
  }
  cout << endl;
};