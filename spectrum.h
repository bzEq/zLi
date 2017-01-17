// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef _ZLI_SPECTRUM_H_
#define _ZLI_SPECTRUM_H_
#include <cmath>
#include <iostream>

#include "color.h"
#include "diagram2d.h"
#include "core_math.h"

namespace zLi {

class Spectrum {
public:
  static const int NrSpectralSamples;
  static const Float DefaultSampleX[];
  static const Float DefaultSampleY[];

  Spectrum();
  explicit Spectrum(Float v);
  Spectrum(const Float *, const Float *, size_t);
  Spectrum(const Spectrum &) = default;

  Spectrum &operator+=(const Spectrum &sp) {
    spd_ += sp.spd_;
    return *this;
  }

  Spectrum &operator*=(const Spectrum &sp) {
    spd_ *= sp.spd_;
    return *this;
  }

  Spectrum &operator*=(Float f) {
    spd_ *= f;
    return *this;
  }

  Spectrum &operator/=(Float f) {
    spd_ *= 1 / f;
    return *this;
  }

  friend Spectrum operator+(const Spectrum &a, const Spectrum &b) {
    return Spectrum(a.spd_ + b.spd_);
  }

  friend Spectrum operator*(const Spectrum &a, const Spectrum &b) {
    return Spectrum(a.spd_ * b.spd_);
  }

  friend Spectrum operator*(const Spectrum &a, Float f) {
    return Spectrum(a.spd_ * f);
  }

  friend Spectrum operator*(Float f, const Spectrum &a) {
    return Spectrum(a.spd_ * f);
  }

  friend Spectrum operator/(const Spectrum &a, Float f) {
    return Spectrum(a.spd_ * (1 / f));
  }

  void Display(std::ostream &out) { return spd_.Display(out); }

  XYZColor ToXYZ() const;
  RGBColor ToRGB() const;
  xyYColor ToxyY() const;

private:
  explicit Spectrum(const Diagram2D &s) : spd_(s) {}
  Diagram2D spd_;  // Spectral power distribution
};
}  // namespace zLi
#endif
