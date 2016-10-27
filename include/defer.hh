#ifndef _ZLI_DEFER_HH_
#define _ZLI_DEFER_HH_
#include <functional>


namespace zLi {

struct Defer {
  std::function<void(void)> f;
  Defer(const std::function<void(void)>& f): f(f) {}
  ~Defer() { f(); }
};
 
} // end namespace zLi

#endif
