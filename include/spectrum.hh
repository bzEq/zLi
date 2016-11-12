#ifndef _ZLI_SPECTRUM_HH_
#define _ZLI_SPECTRUM_HH_
#include "diagram2d.hh"
#include "math.hh"

#include <cmath>
#include <iostream>

namespace zLi {

class Spectrum {
public:
  static const int NrSpectralSamples;
  static const Float DefaultSampleX[];
  static const Float DefaultSampleY[];

  Spectrum();
  Spectrum(Float v);
  Spectrum(const Spectrum &sp) = default;

  Spectrum &operator+=(const Spectrum &sp) {
    spectrum_ += sp.spectrum_;
    return *this;
  }

  Spectrum &operator*=(const Spectrum &sp) {
    spectrum_ *= sp.spectrum_;
    return *this;
  }

  Spectrum &operator*=(Float f) {
    spectrum_ *= f;
    return *this;
  }

  Spectrum &operator/=(Float f) {
    spectrum_ *= 1 / f;
    return *this;
  }

  friend Spectrum operator+(const Spectrum &a, const Spectrum &b) {
    return Spectrum(a.spectrum_ + b.spectrum_);
  }

  friend Spectrum operator*(const Spectrum &a, const Spectrum &b) {
    return Spectrum(a.spectrum_ * b.spectrum_);
  }

  friend Spectrum operator*(const Spectrum &a, Float f) {
    return Spectrum(a.spectrum_ * f);
  }

  friend Spectrum operator*(Float f, const Spectrum &a) {
    return Spectrum(a.spectrum_ * f);
  }

  friend Spectrum operator/(const Spectrum &a, Float f) {
    return Spectrum(a.spectrum_ * (1 / f));
  }

private:
  Spectrum(const Diagram2d &s) : spectrum_(s) {}
  Diagram2d spectrum_;
};
}
#endif
