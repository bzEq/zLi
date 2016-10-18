#include "logging.hh"


namespace zLi {

LogLevel LL;
const char *LLStr[5] = { "INFO", "DEBUG", "WARN", "ERROR", "FATAL" };

void SetLogLevel(LogLevel lv) {
  LL = lv;
}

void Logging(LogLevel level, const char* file, const char* func, int line,
             const char* msg) {
  if (level < LL) return;
  std::fprintf(stderr, "[%s %s:%s:%d] %s\n", LLStr[level], file, func, line, msg);
}

} // namespace zLi
