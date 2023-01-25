#ifndef __TIMER_WHEEL_H
#define __TIMER_WHEEL_H

#include "logger.h"
#include "timerNode.h"
#include "timerSlot.h"
#include <atomic>

namespace _timer {

#define TIMER_SLOT_BIT 10
#define TIMER_SLOT_SIZE (1 << TIMER_SLOT_BIT)
#define TIMER_WHEEL_MASK (TIMER_SLOT_SIZE - 1)

typedef struct {
  List slot[TIMER_SLOT_SIZE];
} Wheel;

class timerWheel {
private:
  timerWheel();

public:
  std::atomic<time_t> jiffy;
  Wheel *wheels[7];
  Wheel wheel1;
  Wheel wheel2;
  Wheel wheel3;
  Wheel wheel4;
  Wheel wheel5;

  virtual ~timerWheel();
  static timerWheel &getInstance();

  time_t update_time(time_t now_time, List &timeouted);

  bool add_timer_node(timerNode &timer_node);
  bool del_timer_node(timerNode &timer_node);
};

}; // namespace _timer

#endif