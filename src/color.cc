// Copyright (c) 2016 Kai Luo. All rights reserved.

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

RGBColor RGBColor::sRGB() const {
  RGBColor rgb;
  for (int i = 0; i < 3; ++i) {
    if (this->operator[](i) < 0.0031308) {
      rgb[i] = 12.92 * this->operator[](i) < 0.0031308;
    } else {
      rgb[i] = 1.055 * std::pow(this->operator[](i), 1 / 2.4) - 0.055;
    }
  }
  return rgb;
}

}  // namespace zLi
