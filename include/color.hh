#ifndef _ZLI_COLOR_HH_
#define _ZLI_COLOR_HH_
#include "math.hh"

#include <iostream>

namespace zLi {

struct xyYColor {
  Float x, y, Y;
  xyYColor() : x(0), y(0), Y(0) {}
  xyYColor(Float x, Float y, Float Y) : x(x), y(y), Y(Y) {}
};

struct RGBColor {
  Float r, g, b;
  RGBColor() : r(0), g(0), b(0) {}
  RGBColor(Float r, Float g, Float b) : r(r), g(g), b(b) {}
  Float &operator[](int i);
  Float operator[](int i) const;
  RGBColor sRGB() const;
};

struct XYZColor {
  Float x, y, z;
  XYZColor() : x(0), y(0), z(0) {}
  XYZColor(Float x, Float y, Float z) : x(x), y(y), z(z) {}
  Float &operator[](int i);
  Float operator[](int i) const;
};

inline std::ostream &operator<<(std::ostream &out, const RGBColor &c) {
  out << "(" << c.r << ", " << c.g << ", " << c.b << ")";
  return out;
}

inline std::ostream &operator<<(std::ostream &out, const XYZColor &c) {
  out << "(" << c.x << ", " << c.y << ", " << c.z << ")";
  return out;
}

inline std::ostream &operator<<(std::ostream &out, const xyYColor &c) {
  out << "(" << c.x << ", " << c.y << ", " << c.Y << ")";
  return out;
}

} // zLi
#endif
