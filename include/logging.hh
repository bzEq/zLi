#ifndef _ZLI_LOGGING_HH_
#define _ZLI_LOGGING_HH_
#include <string>

#define LOG(level, format, ...)                                                \
  zLi::Logger::Logging(level, __FILE__, __FUNCTION__, __LINE__, format,        \
                       ##__VA_ARGS__)

#define INFO(format, ...)                                                      \
  zLi::Logger::Logging(zLi::Logger::INFO, __FILE__, __FUNCTION__, __LINE__,    \
                       format, ##__VA_ARGS__)

#define DEBUG(format, ...)                                                     \
  zLi::Logger::Logging(zLi::Logger::DEBUG, __FILE__, __FUNCTION__, __LINE__,   \
                       format, ##__VA_ARGS__)

#define WARN(format, ...)                                                      \
  zLi::Logger::Logging(zLi::Logger::WARN, __FILE__, __FUNCTION__, __LINE__,    \
                       format, ##__VA_ARGS__)

#define ERROR(format, ...)                                                     \
  zLi::Logger::Logging(zLi::Logger::ERROR, __FILE__, __FUNCTION__, __LINE__,   \
                       format, ##__VA_ARGS__)

#define FATAL(format, ...)                                                     \
  zLi::Logger::Logging(zLi::Logger::FATAL, __FILE__, __FUNCTION__, __LINE__,   \
                       format, ##__VA_ARGS__)

namespace zLi {

class Logger {
public:
  enum LogLevel { INFO, DEBUG, WARN, ERROR, FATAL };

  static void SetLogLevel(LogLevel);

  static void Logging(LogLevel level, const char *file, const char *func,
                      int line, std::string &&msg);

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
    char *buf = (char *)malloc(cap);
    while (cap <= LIMITS) {
      int len = snprintf(buf, cap, fmt, std::forward<Args>(args)...);
      if (len < cap) {
        break;
      }
      free(buf);
      cap <<= 1;
      buf = (char *)malloc(cap);
    }
    Logging(level, file, func, line, buf);
    free(buf);
    return;
  }

private:
  static LogLevel CurrentLevel;
  static const char *LogLevelString[5];

  template <typename... Args>
  static std::string FormatToString(const char *fmt, Args &&... args) {
    static const int LIMITS = (1 << 16);
    int cap = 64;
    char *buf = (char *)malloc(cap);
    while (cap <= LIMITS) {
      int len = snprintf(buf, cap, fmt, std::forward<Args>(args)...);
      if (len < cap) {
        break;
      }
      free(buf);
      cap <<= 1;
      buf = (char *)malloc(cap);
    }
    std::string s(buf);
    free(buf);
    return s;
  }
};
} // namespace zLi

#endif
