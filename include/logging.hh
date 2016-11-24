// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef _ZLI_LOGGING_HH_
#define _ZLI_LOGGING_HH_
#include <mutex>
#include <string>
#include <utility>

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
    static const int LIMITS = (1 << 16);
    if (level < CurrentLevel) {
      return;
    }
    int cap = 64;
    char *buf = new char[cap];
    while (cap <= LIMITS) {
      int len = snprintf(buf, cap, fmt, std::forward<Args>(args)...);
      if (len < cap) {
        break;
      }
      delete[] buf;
      cap <<= 1;
      buf = new char[cap];
    }
    Logging(level, file, func, line, buf);
    delete[] buf;
    return;
  }

private:
  static std::mutex OutputMutex;
  static LogLevel CurrentLevel;
  static const char *LogLevelString[5];
};
}  // namespace zLi

#endif
