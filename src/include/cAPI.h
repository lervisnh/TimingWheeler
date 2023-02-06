#pragma once

#include "timerDriver.h"
#include "timerEvent.h"
#include "timerWheel.h"
#include <unistd.h>

namespace timingWheeler {

inline void init_timer() {
  _timer::timerDriver::get_instance();
  usleep(1);
};

template <typename F, typename... Args,
          typename timerEvent = _timerEvent::timerEvent<F, Args...>>
unsigned long int register_timer(_timer::counter_t repeat_num,
                                 _timer::time_t interval /* 100us */, F &&f,
                                 Args &&...args) {
  auto &&driver = _timer::timerDriver::get_instance();
  auto expire_interval =
      driver.cal_expires(_timer::timerDriver::TimePoint::clock::now() +
                         _timer::timerDriver::Duration{interval * 100000});
  auto *ev = new timerEvent{repeat_num, expire_interval, std::forward<F>(f),
                            std::forward<Args>(args)...};
  _timer::timerWheel::get_instance().add_timer_node(*ev);
  return ev->timerID();
};

inline bool unregister_timer(unsigned long int timer_id) {
  return _timerEvent::timerEventObject::remove_timer_event(timer_id);
};

}; // namespace timingWheeler