#ifndef _ZLI_SPECTRUM_HH_
#define _ZLI_SPECTRUM_HH_
#include "math.hh"

#include <cmath>
#include <iostream>

namespace zLi {

static const int nrSpectralSamples = 10;

struct Spectrum {
  Float s[nrSpectralSamples];
  Spectrum() {
    for (int i = 0; i < nrSpectralSamples; i++)
      s[i] = 0;
  }
  Spectrum(Float v) {
    for (int i = 0; i < nrSpectralSamples; i++)
      s[i] = v;
  }
  Spectrum(const Spectrum &sp) {
    for (int i = 0; i < nrSpectralSamples; i++)
      s[i] = sp.s[i];
  }
  Spectrum(const Float v[nrSpectralSamples]) {
    for (int i = 0; i < nrSpectralSamples; i++)
      s[i] = v[i];
  }
  Spectrum &operator+=(const Spectrum &sp) {
    for (int i = 0; i < nrSpectralSamples; i++)
      s[i] += sp.s[i];
    return *this;
  }
  Spectrum &operator*=(const Spectrum &sp) {
    for (int i = 0; i < nrSpectralSamples; i++)
      s[i] *= sp.s[i];
    return *this;
  }
  Spectrum &operator*=(Float f) {
    assert(f >= 0);
    for (int i = 0; i < nrSpectralSamples; i++)
      s[i] *= f;
    return *this;
  }
  Spectrum &operator/=(Float f) {
    assert(f > 0);
    for (int i = 0; i < nrSpectralSamples; i++)
      s[i] /= f;
    return *this;
  }
  Float NormSquared() const {
    Float sum = 0;
    for (int i = 0; i < nrSpectralSamples; i++)
      sum += s[i] * s[i];
    return sum;
  }
  Float Norm() const { return std::sqrt(NormSquared()); }
};

inline Spectrum operator+(const Spectrum &a, const Spectrum &b) {
  Spectrum c;
  for (int i = 0; i < nrSpectralSamples; i++)
    c.s[i] = a.s[i] + b.s[i];
  return c;
}

inline Spectrum operator*(const Spectrum &a, const Spectrum &b) {
  Spectrum c;
  for (int i = 0; i < nrSpectralSamples; i++)
    c.s[i] = a.s[i] * b.s[i];
  return c;
}

inline Spectrum operator*(const Spectrum &a, Float f) {
  assert(f >= 0);
  Spectrum c;
  for (int i = 0; i < nrSpectralSamples; i++)
    c.s[i] = a.s[i] * f;
  return c;
}

inline Spectrum operator*(Float f, const Spectrum &a) {
  Spectrum c;
  for (int i = 0; i < nrSpectralSamples; i++)
    c.s[i] = a.s[i] * f;
  return c;
}

inline Spectrum operator/(const Spectrum &a, Float f) {
  assert(f > 0);
  Spectrum c;
  for (int i = 0; i < nrSpectralSamples; i++)
    c.s[i] = a.s[i] / f;
  return c;
}
}
#endif
