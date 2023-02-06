#pragma once

#include "bitTypes.h"
#include <cassert>

namespace _timer {

class List;

struct NODE {
  struct NODE *prev{0x0};
  struct NODE *next{0x0};
  virtual ~NODE() = default;
};

typedef struct NODE Node;

class timerNode : public Node {
private:
  timerNode() = delete;

public:
  List *belonged_wheel_slot{nullptr};
  time_t expire;

  explicit timerNode(time_t exp) { expire = exp; };

  virtual bool repeat_execute() {
    assert(false);
    return false;
  };
};

class noncopyable {
  // https://github.com/chenshuo/muduo/blob/master/muduo/base/noncopyable.h
public:
  noncopyable(const noncopyable &) = delete;
  void operator=(const noncopyable &) = delete;

protected:
  noncopyable() = default;
  ~noncopyable() = default;
};

}; // namespace _timer