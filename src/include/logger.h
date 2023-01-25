#pragma once

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <atomic>
#include <utility>

namespace _logger {

class Logger {
private:
  using LoggerImpl = std::shared_ptr<spdlog::logger>;
  using SpdLogLevel = spdlog::level::level_enum;

  LoggerImpl impl;

protected:
  static std::atomic<Logger *> singleton;

public:
  enum Level : int {
    trace = SpdLogLevel::trace,
    debug = SpdLogLevel::debug,
    info = SpdLogLevel::info,
    warning = SpdLogLevel::warn,
    error = SpdLogLevel::err,
    fatal = SpdLogLevel::critical
  };

  Logger() = delete;
  ~Logger() = default;

  Logger(const char *log_file);

  LoggerImpl &getImpl();

  static bool initSingleton(const char *log_file);
  static void resetSingleton(const char *log_file = nullptr);
  static Logger &getSingleton();
};
}; // namespace _logger

namespace LOG {

bool init(const char *log_file = "logs/TimingWheeler.log");
void reset(const char *log_file = nullptr);

template <typename... Args> void trace(Args... args) {
  _logger::Logger::getSingleton().getImpl()->trace(args...);
};
template <typename... Args> void debug(Args... args) {
  _logger::Logger::getSingleton().getImpl()->debug(args...);
};
template <typename... Args> void info(Args... args) {
  _logger::Logger::getSingleton().getImpl()->info(args...);
};
template <typename... Args> void warn(Args... args) {
  _logger::Logger::getSingleton().getImpl()->warn(args...);
};
template <typename... Args> void error(Args... args) {
  _logger::Logger::getSingleton().getImpl()->error(args...);
};
template <typename... Args> void critical(Args... args) {
  _logger::Logger::getSingleton().getImpl()->critical(args...);
};

}; // namespace LOG