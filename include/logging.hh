#ifndef _ZLI_LOGGING_HH_
#define _ZLI_LOGGING_HH_
#include <string>

#define LOG(level, format, ...)                                                \
  zLi::Logging(level, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#define INFO(format, ...)                                                      \
  zLi::Logging(zLi::INFO, __FILE__, __FUNCTION__, __LINE__, format,            \
               ##__VA_ARGS__)

#define DEBUG(format, ...)                                                     \
  zLi::Logging(zLi::DEBUG, __FILE__, __FUNCTION__, __LINE__, format,           \
               ##__VA_ARGS__)

#define WARN(format, ...)                                                      \
  zLi::Logging(zLi::WARN, __FILE__, __FUNCTION__, __LINE__, format,            \
               ##__VA_ARGS__)

#define ERROR(format, ...)                                                     \
  zLi::Logging(zLi::ERROR, __FILE__, __FUNCTION__, __LINE__, format,           \
               ##__VA_ARGS__)

#define FATAL(format, ...)                                                     \
  zLi::Logging(zLi::FATAL, __FILE__, __FUNCTION__, __LINE__, format,           \
               ##__VA_ARGS__)

namespace zLi {

template <typename... Args>
inline std::string FormatToString(const char *fmt, Args &&... args) {
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

enum LogLevel { INFO, DEBUG, WARN, ERROR, FATAL };

extern LogLevel LL;
extern const char *LLStr[5];

void SetLogLevel(LogLevel);

void Logging(LogLevel level, const char *file, const char *func, int line,
             std::string &&msg);

void Logging(LogLevel level, const char *file, const char *func, int line,
             const char *msg);

template <typename... Args>
inline static void Logging(LogLevel level, const char *file, const char *func,
                           int line, const char *fmt, Args &&... args) {
  static const int LIMITS = (1 << 16);
  if (level < LL) {
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
} // namespace zLi

#endif
