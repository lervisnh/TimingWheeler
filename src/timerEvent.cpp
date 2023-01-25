#include "timerEvent.h"

namespace _timerEvent {

std::atomic_uint_least64_t timerEventObject::counter{0};
std::map<_bitTypes::uint64_t, timerEventObject *> timerEventObject::useing{};
std::set<timerEventObject *> timerEventObject::expired{};

}; // namespace _timerEvent