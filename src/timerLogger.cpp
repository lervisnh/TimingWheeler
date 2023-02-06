#include "timerLogger.h"
#include "spdlog/common.h"

namespace _logger {

std::atomic<Logger *> Logger::singleton{nullptr};

Logger::Logger(const char *log_file)
    : impl(spdlog::basic_logger_mt<spdlog::async_factory>("timerLogger",
                                                          log_file)) {
#if NDEBUG
  impl->set_level(spdlog::level::warn);
  impl->set_pattern("[%Y-%m-%d %H:%M:%S.%f] [%l] %v");
#else
  impl->set_level(spdlog::level::trace);
  impl->set_pattern("[%Y-%m-%d %H:%M:%S.%f] [%i] [tid %t] [%l] %v");
#endif
};

Logger::LoggerImpl &Logger::get_impl() { return impl; };

bool Logger::init_singleton(const char *log_file) {
  if (!Logger::singleton.load()) {
    Logger::singleton.exchange(new Logger(log_file));
    return true;
  }
  return false;
};

void Logger::reset_singleton(const char *log_file) {
  _logger::Logger *old_singleton = Logger::singleton.load();
  Logger::singleton.exchange(nullptr);
  if (log_file) {
    assert(Logger::init_singleton(log_file) == true);
  }
  if (old_singleton) {
    delete old_singleton;
  }
};

Logger &Logger::get_singleton() { return *(Logger::singleton.load()); };

}; // namespace _logger

namespace LOG {

bool init(const char *log_file) {
  return _logger::Logger::init_singleton(log_file);
};

void reset(const char *log_file) {
  _logger::Logger::reset_singleton(log_file);
};

}; // namespace LOG