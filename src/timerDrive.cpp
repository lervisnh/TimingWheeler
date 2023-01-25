#include "bitTypes.h"
#include "logger.h"
#include "timerDriver.h"
#include "timerEvent.h"
#include "timerNode.h"
#include "timerWheel.h"
#include <ctime>
#include <ratio>
#include <thread>
#include <unistd.h>

namespace _timer {

void Driver::start() {
  running.exchange(true);
  startns = TimePoint::clock::now();
  while (running) {
    this->tik();
    // if ((TimePoint::clock::now() - startns).count() >=
    //     TICK_INTERVAL_US * 1000) {
    //   this->tik();
    // }
    // this->tik();
    // usleep(TICK_INTERVAL_US - (TICK_INTERVAL_US >> 2));
  };
};

void Driver::stop() {
  running.exchange(false);
  this->thread.join();
  _timerEvent::timerEventObject::clear_all();
};
Driver::~Driver() { stop(); };

Driver::Driver(timerWheel *timer_wheel)
    : timerwheel(timer_wheel ? timer_wheel
                             : &_timer::timerWheel::getInstance()),
      running(false), thread([this] { this->start(); }){};

Driver &Driver::getInstance() {
  static Driver instance(nullptr);
  return instance;
};

void Driver::tik() {
  List timeouted;
  timerwheel->update_time(cal_expires(TimePoint::clock::now()), timeouted);
  _timer::timerNode *n = nullptr;
  while (timeouted.pop_back(n)) {
    LOG::trace("call expire = {}", n->expire);
    if (n->repeat_execute()) {
      timerwheel->add_timer_node(*n);
    }

    if (_timerEvent::timerEventObject::expired.size() > 100000) {
      _timerEvent::timerEventObject::clear_expired();
    }
  }
};

_timer::time_t Driver::cal_expires(TimePoint &&time_point) {
  return (time_point - startns).count() /*ns*/ / tik_ns;
};

}; // namespace _timer