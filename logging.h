// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef _ZLI_LOGGING_H_
#define _ZLI_LOGGING_H_
#include <atomic>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#define ZLOG(level, format, ...)                                               \
  zLi::Logger::Logging(level, __FILE__, __FUNCTION__, __LINE__, format,        \
                       ##__VA_ARGS__)

#define INFOLOG(format, ...)                                                   \
  zLi::Logger::Logging(zLi::Logger::INFO, __FILE__, __FUNCTION__, __LINE__,    \
                       format, ##__VA_ARGS__)

#define DEBUGLOG(format, ...)                                                  \
  zLi::Logger::Logging(zLi::Logger::DEBUG, __FILE__, __FUNCTION__, __LINE__,   \
                       format, ##__VA_ARGS__)

#define WARNLOG(format, ...)                                                   \
  zLi::Logger::Logging(zLi::Logger::WARN, __FILE__, __FUNCTION__, __LINE__,    \
                       format, ##__VA_ARGS__)

#define ERRORLOG(format, ...)                                                  \
  zLi::Logger::Logging(zLi::Logger::ERROR, __FILE__, __FUNCTION__, __LINE__,   \
                       format, ##__VA_ARGS__)

#define FATALLOG(format, ...)                                                  \
  zLi::Logger::Logging(zLi::Logger::FATAL, __FILE__, __FUNCTION__, __LINE__,   \
                       format, ##__VA_ARGS__)

namespace zLi {

class Logger {
public:
  enum LogLevel { INFO, DEBUG, WARN, ERROR, FATAL };

  static void SetLogLevel(LogLevel);

  static void Logging(LogLevel level, const char *file, const char *func,
                      int line, const char *msg);

  template <typename... Args>
  static void Logging(LogLevel level, const char *file, const char *func,
                      int line, const char *fmt, Args &&... args) {
    static const size_t size_limit = (1 << 16);
    if (level < CurrentLevel) {
      return;
    }
    size_t size = std::snprintf(nullptr, 0, fmt, std::forward<Args>(args)...);
    std::vector<char> buf(std::min(size_limit, size + 1));
    std::snprintf(&buf[0], buf.size(), fmt, std::forward<Args>(args)...);
    return Logging(level, file, func, line, buf.data());
  }

private:
  static std::mutex OutputMutex;
  static const char *LogLevelString[5];
  static std::atomic<LogLevel> CurrentLevel;
};
}  // namespace zLi

#endif
