#pragma once

#include "bitTypes.h"
#include "timerWheel.h"
#include <atomic>
#include <chrono>
#include <ctime>
#include <ratio>
#include <thread>

namespace _timer {

#define TICK_INTERVAL_US 100
class timerDriver {
public:
  using Clock = std::chrono::steady_clock;
  using Duration = std::chrono::duration<_timer::time_t, std::nano>;
  using TimePoint = std::chrono::time_point<Clock, Duration>;

private:
  const _timer::time_t tik_ns_{TICK_INTERVAL_US * 1000};
  timerWheel *timerwheel_ptr_;
  std::atomic_bool running_;
  TimePoint startns_;
  std::thread thread_;

protected:
  void start();
  void stop();
  void tik();

public:
  timerDriver() = delete;
  explicit timerDriver(timerWheel *);
  ~timerDriver();

  _timer::time_t cal_expires(TimePoint &&);
  void immediately_stop();

  static timerDriver &get_instance();
};

}; // namespace _timer