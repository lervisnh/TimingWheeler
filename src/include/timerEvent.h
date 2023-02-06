#pragma once

#include <atomic>
#include <cstddef>
#include <functional>
#include <future>
#include <map>
#include <memory>
#include <set>
#include <type_traits>
#include <utility>

#include "timerLogger.h"
#include "timerWheel.h"

#if __cplusplus < 201402L
namespace std {
template <class T>
using remove_reference_t = typename remove_reference<T>::type;
}
#endif
#if __cplusplus < 201703L
namespace std {
template <class T> using result_of_t = typename std::result_of<T>::type;
}
#endif

using _timer::noncopyable;
using _timer::timerWheel;

namespace _timerEvent {

using _timer::counter_t;
using _timer::time_t;
using _timer::timerNode;

// https://ideone.com/Nj6rGb/

namespace _helper {
template <std::size_t... Ts> struct index {};

template <std::size_t N, std::size_t... Ts>
struct gen_seq : gen_seq<N - 1, N - 1, Ts...> {};

template <std::size_t... Ts> struct gen_seq<0, Ts...> : index<Ts...> {};
}; // namespace _helper

class timerEventObject : public timerNode, public noncopyable {
private:
  const _bitTypes::uint64_t id;

public:
  static std::atomic_uint_least64_t counter;
  static std::map<_bitTypes::uint64_t, timerEventObject *> useing;
  static std::set<timerEventObject *> expired;

  static bool remove_timer_event(_bitTypes::uint64_t id) {
    if (id > counter ||
        timerEventObject::useing.find(id) == timerEventObject::useing.end()) {
      return false;
    }
    auto &ev = timerEventObject::useing[id];
    // remove from wheel-slot
    timerWheel::get_instance().del_timer_node(*ev);
    delete ev;
    timerEventObject::useing.erase(id);
    return true;
  };

  static size_t clear_expired() {
    size_t expired_num = expired.size();
    for (auto &&exped : timerEventObject::expired) {
      delete exped;
    }
    timerEventObject::expired.clear();
    return expired_num;
  };

  static void clear_all() {
    clear_expired();
    for (auto &&u : timerEventObject::useing) {
      delete u.second;
    }
    timerEventObject::useing.clear();
  };

  explicit timerEventObject(time_t exp)
      : timerNode(exp), id(++timerEventObject::counter) {
    timerEventObject::useing[id] = this;
  };
  virtual ~timerEventObject() {
    LOG::trace("virtual destructor timerEventObject");
  };

  _bitTypes::uint64_t timerID() { return id; };
};

template <typename F, typename... ARG>
class timerEvent : public timerEventObject {
private:
  using FunctionArgsType = std::tuple<std::remove_reference_t<ARG>...>;
  using FunctionRetType = typename std::result_of_t<F(ARG...)>;
  using FunctionType = std::function<FunctionRetType(ARG...)>;

  counter_t repeat;
  time_t interval_expire;

  FunctionType func;
  FunctionArgsType args;

  template <typename... Args, std::size_t... Is>
  FunctionRetType function(std::tuple<Args...> &tup, _helper::index<Is...>) {
    return func(std::get<Is>(tup)...);
  }

  template <typename... Args>
  FunctionRetType function(std::tuple<Args...> &tup) {
    return function(tup, _helper::gen_seq<sizeof...(Args)>{});
  }

public:
  timerEvent() = delete;

  timerEvent(counter_t repeat_num, time_t interval, F &&f, ARG &&...argv)
      : timerEventObject(interval), repeat(repeat_num),
        interval_expire(interval), func(std::forward<F>(f)),
        args(std::make_tuple(std::forward<ARG>(argv)...)){};

  FunctionRetType operator()() { return function(args); };

  counter_t execute() {
    (*this)();
    return --repeat;
  };

  bool repeat_execute() {
    if (execute() > 0) {
      LOG::trace("remaining repeat = {}", repeat);
      expire += interval_expire;
      return true;
    }
    timerEventObject::useing.erase(timerID());
    timerEventObject::expired.insert(this);
    return false;
  };
};

template <typename F, typename... Args,
          typename timerEvent = timerEvent<F, Args...>>
timerEvent Make(counter_t repeat_num, time_t interval, F &&f, Args &&...args) {
  return {repeat_num, interval, std::forward<F>(f),
          std::forward<Args>(args)...};
};

template <typename F, typename... Args,
          typename timerEvent = timerEvent<F, Args...>>
timerEvent *New(counter_t repeat_num, time_t interval, F &&f, Args &&...args) {
  return new timerEvent{repeat_num, interval, std::forward<F>(f),
                        std::forward<Args>(args)...};
};
template <typename timerEvent> void Delete(timerEvent *cb) { delete cb; };

}; // namespace _timerEvent