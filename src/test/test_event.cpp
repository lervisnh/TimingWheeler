#include "timerEvent.h"
#include <iostream>

using std::cout;
using std::endl;

#define line_print(x)                                                          \
  { cout << #x << "\t= " << x << endl; }

int add(int x, int y) {
  line_print(x + y);
  return x + y;
}

void ffff() { cout << "ffff" << endl; }

int main() {
  LOG::init("test_event");

  auto &&f = _timerEvent::Make(1, 1, add, 10, 20);
  f();

  auto lmd = [] { line_print(0.0009999); };
  auto f2 = _timerEvent::New(1, 1, lmd);
  (*f2)();
  _timerEvent::Delete(f2);

  auto &&f3 = _timerEvent::Make(1, 1, ffff);
  f3();
  f3();

  // LOG::reset();
};
