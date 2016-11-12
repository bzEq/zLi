#include "color.hh"

namespace zLi {

Float &Color::operator[](int i) {
  assert(i >= 0 && i < 4);
  switch (i) {
  case 0:
    return r;
  case 1:
    return g;
  case 2:
    return a;
  }
  return a;
}

} // zLi
