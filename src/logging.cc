// Copyright (c) 2016 Kai Luo. All rights reserved.

#include "logging.hh"

namespace zLi {

std::mutex Logger::OutputMutex;

std::atomic<Logger::LogLevel> Logger::CurrentLevel(Logger::INFO);

const char *Logger::LogLevelString[5] = {"INFO", "DEBUG", "WARN", "ERROR",
                                         "FATAL"};

void Logger::SetLogLevel(Logger::LogLevel level) { CurrentLevel = level; }

void Logger::Logging(Logger::LogLevel level, const char *file, const char *func,
                     int line, const char *msg) {
  if (level < CurrentLevel) {
    return;
  }
  OutputMutex.lock();
  std::fprintf(stderr, "[%s %s:%s:%d] %s\n", LogLevelString[level], file, func,
               line, msg);
  OutputMutex.unlock();
}

}  // namespace zLi
