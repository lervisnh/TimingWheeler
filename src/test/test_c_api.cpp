#include "c_api.h"

#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;

static std::vector<
    std::chrono::duration<long, struct std::ratio<1, 1000000000>>::rep>
    tms;
static std::vector<unsigned long int> timer_ids;

void timer_node_print(_timer::time_t idx) {
  // cout << idx << "\t" << endl;
  auto now = std::chrono::system_clock::now().time_since_epoch().count();
  LOG::trace("timer_node_print  {} \t {}", idx, now);
  tms.emplace_back(now);
};

int main() {
  timerWheeler::init_timer();
  cout << timerWheeler::register_timer(2, 10, timer_node_print, 10) << endl;
  cout << timerWheeler::register_timer(1, 0, timer_node_print, 0) << endl;
  cout << timerWheeler::register_timer(1, 99, timer_node_print, 99) << endl;

  auto remove_timer = timerWheeler::register_timer(1, 59, timer_node_print, 59);
  cout << "unregister " << remove_timer << " : " << std::boolalpha
       << timerWheeler::unregister_timer(remove_timer) << endl;

  sleep(3);

  cout << "timer event using-size = "
       << _timerEvent::timerEventObject::useing.size()
       << ",\t expired-size = " << _timerEvent::timerEventObject::expired.size()
       << endl;
  return 0;
};