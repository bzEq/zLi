// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef _ZLI_DEFER_H_
#define _ZLI_DEFER_H_
#include <functional>

namespace zLi {

struct Defer {
  explicit Defer(const std::function<void(void)> &f) : f(f) {}
  std::function<void(void)> f;
  ~Defer() { f(); }
};

}  // end namespace zLi

#endif
