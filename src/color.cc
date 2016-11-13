#include "color.hh"

namespace zLi {

Float &XYZColor::operator[](int i) {
  assert(i >= 0 && i < 3);
  switch (i) {
  case 0:
    return x;
  case 1:
    return y;
  case 2:
    return z;
  }
  return x;
}

Float XYZColor::operator[](int i) const {
  assert(i >= 0 && i < 3);
  switch (i) {
  case 0:
    return x;
  case 1:
    return y;
  case 2:
    return z;
  }
  return x;
}

Float &RGBColor::operator[](int i) {
  assert(i >= 0 && i < 3);
  switch (i) {
  case 0:
    return r;
  case 1:
    return g;
  case 2:
    return b;
  }
  return r;
}

Float RGBColor::operator[](int i) const {
  assert(i >= 0 && i < 3);
  switch (i) {
  case 0:
    return r;
  case 1:
    return g;
  case 2:
    return b;
  }
  return r;
}

RGBColor RGBColor::Clamp() const {
  return RGBColor(std::min(std::max(r, 0.f), 1.f),
                  std::min(std::max(g, 0.f), 1.f),
                  std::min(std::max(b, 0.f), 1.f));
}

} // zLi
