#pragma once
// https://github.com/chenshuo/muduo/blob/master/muduo/base/noncopyable.h

class noncopyable {
public:
  noncopyable(const noncopyable &) = delete;
  void operator=(const noncopyable &) = delete;

protected:
  noncopyable() = default;
  ~noncopyable() = default;
};