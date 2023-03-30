#include "timerLogger.h"
#include "spdlog/common.h"

namespace _logger {

std::atomic<Logger *> Logger::singleton{nullptr};

Logger::Logger(const char *log_file)
    : impl_(spdlog::basic_logger_mt<spdlog::async_factory>("timerLogger",
                                                          log_file)) {
#if NDEBUG
  impl_->set_level(spdlog::level::warn);
  impl_->set_pattern("[%Y-%m-%d %H:%M:%S.%f] [%l] %v");
#else
  impl_->set_level(spdlog::level::trace);
  impl_->set_pattern("[%Y-%m-%d %H:%M:%S.%f] [%i] [tid %t] [%l] %v");
#endif
};

Logger::LoggerImpl &Logger::get_impl() { return impl_; };

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