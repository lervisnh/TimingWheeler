#include "timerWheel.h"
#include "bitTypes.h"
#include "timerLogger.h"
#include <algorithm>
#include <cassert>
#include <utility>

namespace _timer {

timerWheel::timerWheel() : jiffy(0) {
  LOG::init();
  LOG::trace("init wheels, jiffy = {}", jiffy.load());
  wheels[0] = wheels[6] = nullptr;
  wheels[1] = &wheel1;
  wheels[2] = &wheel2;
  wheels[3] = &wheel3;
  wheels[4] = &wheel4;
  wheels[5] = &wheel5;

  for (int i = 1; i < 6; ++i) {
    for (int j = 0; j < TIMER_SLOT_SIZE; ++j) {
      wheels[i]->slot[j].init();
    }
  }
};

timerWheel::~timerWheel() { LOG::reset(); };

timerWheel &timerWheel::get_instance() {
  static timerWheel instance;
  return instance;
};

namespace _internal {

constexpr _timer::time_t MAX_JIFFY = 0x3FFFFFFFFFFFF;

index_t timer_wheel_index(timerWheel *core, index_t wheel_level) {
  return ((core->jiffy.load()) >> (wheel_level * TIMER_SLOT_BIT)) &
         TIMER_WHEEL_MASK;
};

void wheel_cascade(timerWheel *core, index_t wheel_idx, index_t slot_idx) {
  auto &wheel = core->wheels[wheel_idx];
  auto &slot_list = wheel->slot[slot_idx];
  List timeouted_list{};
  timeouted_list.swap(slot_list);
  // LOG::trace("wheel {} slot {} to cascade, node num = {}", wheel_idx,
  // slot_idx,
  //            timeouted_list.size());
  _timer::timerNode *n = nullptr;
  while (timeouted_list.pop_back(n)) {
    core->add_timer_node(*n);
  }
};

List timer_wheels_update(timerWheel *core, time_t jiffies) {
  List timeouted_list{};
  while (core->jiffy <= jiffies) {
    index_t index = (core->jiffy.load()) & TIMER_WHEEL_MASK;
    if (index == 0) {
      index_t i = timer_wheel_index(core, 1);
      wheel_cascade(core, 2, i);
      if (i == 0) {
        i = timer_wheel_index(core, 2);
        wheel_cascade(core, 3, i);
        if (i == 0) {
          i = timer_wheel_index(core, 3);
          wheel_cascade(core, 4, i);
          if (i == 0) {
            i = timer_wheel_index(core, 3);
            wheel_cascade(core, 5, i);
          }
        }
      }
    }
    auto &timeout_slot_list = core->wheel1.slot[index];
    LOG::trace("jiffy {} timer wheels updating to {}, hit slot-index = {} (in "
               "wheel-1) node-num = {}",
               core->jiffy.load(), jiffies, index, timeout_slot_list.size());

    core->jiffy.fetch_add(1);
    timeouted_list.merge_back(timeout_slot_list);
  }

  core->jiffy.fetch_sub(1);

  LOG::trace("timer current jiffy = {}, timeout node-num = {}",
             core->jiffy.load(), timeouted_list.size());
  return {timeouted_list};
};
}; // namespace _internal

time_t timerWheel::update_time(time_t now_time, List &timeouted) {
  timeouted = _internal::timer_wheels_update(this, now_time);
  assert(this->jiffy.load() == now_time);
  return jiffy;
};

bool timerWheel::add_timer_node(timerNode &timer_node) {
  LOG::trace("add expire = {} node to wheels, jiffy = {}", timer_node.expire,
             jiffy.load());
  if (timer_node.expire < this->jiffy ||
      timer_node.expire > _internal::MAX_JIFFY) {
    LOG::warn("add node expire = {} failed, jiffy = {} (max-jiffy={})",
              timer_node.expire, this->jiffy.load(), _internal::MAX_JIFFY);
    return false;
  }

  _timer::time_t time_diff = timer_node.expire - this->jiffy;
  List *wheel_slot = nullptr;

  int i;
  int hit_wheel_level;
  if (time_diff < TIMER_SLOT_SIZE) {
    i = timer_node.expire & TIMER_WHEEL_MASK;
    wheel_slot = this->wheel1.slot + i;
    hit_wheel_level = 1;
  } else if (time_diff < (1UL << (TIMER_SLOT_BIT * 2))) {
    i = (timer_node.expire >> TIMER_SLOT_BIT) & TIMER_WHEEL_MASK;
    wheel_slot = this->wheel2.slot + i;
    hit_wheel_level = 2;
  } else if (time_diff < (1UL << (TIMER_SLOT_BIT * 3))) {
    i = (timer_node.expire >> (TIMER_SLOT_BIT * 2)) & TIMER_WHEEL_MASK;
    wheel_slot = this->wheel3.slot + i;
    hit_wheel_level = 3;
  } else if (time_diff < (1UL << (TIMER_SLOT_BIT * 4))) {
    i = (timer_node.expire >> (TIMER_SLOT_BIT * 3)) & TIMER_WHEEL_MASK;
    wheel_slot = this->wheel4.slot + i;
    hit_wheel_level = 4;
  } else {
    i = (timer_node.expire >> (TIMER_SLOT_BIT * 4)) & TIMER_WHEEL_MASK;
    wheel_slot = this->wheel5.slot + i;
    hit_wheel_level = 5;
  }

  LOG::trace("expire {} node added to wheel {} slot {}", timer_node.expire,
             hit_wheel_level, i);

  timer_node.belonged_wheel_slot = wheel_slot;
  // timer_node.belonged_core = this;
  wheel_slot->add_back(timer_node);

  return true;
};

bool timerWheel::del_timer_node(timerNode &timer_node) {
  // self-steal to delete from list
  // TOUPDATE : move to list's ops
  if (timer_node.next && timer_node.prev) {
    timer_node.next->prev = timer_node.prev;
    timer_node.prev->next = timer_node.next;
    timer_node.next = nullptr;
    timer_node.prev = nullptr;
    timer_node.belonged_wheel_slot->counter--;
    return true;
  };
  return false;
};

}; // namespace _timer
