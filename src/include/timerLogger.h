#pragma once

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <atomic>
#include <utility>

namespace _logger {

class Logger {
private:
  using LoggerImpl = std::shared_ptr<spdlog::logger>;

  LoggerImpl impl_;

protected:
  static std::atomic<Logger *> singleton;

public:
  Logger() = delete;
  ~Logger() = default;

  Logger(const char *log_file);

  LoggerImpl &get_impl();

  static bool init_singleton(const char *log_file);
  static void reset_singleton(const char *log_file = nullptr);
  static Logger &get_singleton();
};
}; // namespace _logger

namespace LOG {

bool init(const char *log_file = "logs/timingWheeler.log");
void reset(const char *log_file = nullptr);

template <typename... Args> void trace(Args... args) {
  _logger::Logger::get_singleton().get_impl()->trace(args...);
};
template <typename... Args> void debug(Args... args) {
  _logger::Logger::get_singleton().get_impl()->debug(args...);
};
template <typename... Args> void info(Args... args) {
  _logger::Logger::get_singleton().get_impl()->info(args...);
};
template <typename... Args> void warn(Args... args) {
  _logger::Logger::get_singleton().get_impl()->warn(args...);
};
template <typename... Args> void error(Args... args) {
  _logger::Logger::get_singleton().get_impl()->error(args...);
};
template <typename... Args> void critical(Args... args) {
  _logger::Logger::get_singleton().get_impl()->critical(args...);
};

}; // namespace LOG