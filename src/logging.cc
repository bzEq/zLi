#include "logging.hh"

namespace zLi {

Logger::LogLevel Logger::CurrentLevel = Logger::INFO;

const char *Logger::LogLevelString[5] = {"INFO", "DEBUG", "WARN", "ERROR",
                                         "FATAL"};

void Logger::SetLogLevel(Logger::LogLevel lv) { CurrentLevel = lv; }

void Logger::Logging(Logger::LogLevel level, const char *file, const char *func,
                     int line, const char *msg) {
  if (level < CurrentLevel) {
    return;
  }
  std::fprintf(stderr, "[%s %s:%s:%d] %s\n", LogLevelString[level], file, func,
               line, msg);
}

} // namespace zLi
