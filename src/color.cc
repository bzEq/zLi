// Copyright (c) 2016 Kai Luo. All rights reserved.

#include "color.hh"

namespace zLi {

Float &XYZColor::operator[](int i) {
  assert(i >= 0 && i < 3);
  return (&x)[i];
}

Float XYZColor::operator[](int i) const {
  assert(i >= 0 && i < 3);
  return (&x)[i];
}

Float &RGBColor::operator[](int i) {
  assert(i >= 0 && i < 3);
  return (&r)[i];
}

Float RGBColor::operator[](int i) const {
  assert(i >= 0 && i < 3);
  return (&r)[i];
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
