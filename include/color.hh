#ifndef _ZLI_COLOR_HH_
#define _ZLI_COLOR_HH_
#include "math.hh"

namespace zLi {
struct Color {
  Float r, g, b, a;
  Color() : r(0), g(0), b(0), a(0) {}
  Float &operator[](int i);
};
} // zLi
#endif
