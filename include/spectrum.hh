#ifndef _ZLI_SPECTRUM_H_
#define _ZLI_SPECTRUM_H_
#include "geometry.hh"

#include <iostream>
#include <cmath>


namespace zLi {

const int nrSpectrumSamples = 4;
struct Spectrum {
  Float s[nrSpectrumSamples];
  Spectrum() { for (int i = 0; i < nrSpectrumSamples; i++) s[i] = 0; }
  Spectrum(const Float v) { for (int i = 0; i < nrSpectrumSamples; i++) s[i] = v; }
  Spectrum(const Spectrum& sp) { for (int i = 0; i < nrSpectrumSamples; i++) s[i] = sp.s[i]; }
  Spectrum(const Float v[nrSpectrumSamples]) { for (int i = 0; i < nrSpectrumSamples; i++) s[i] = v[i]; }
  Spectrum& operator+=(const Spectrum& sp) { for (int i = 0; i < nrSpectrumSamples; i++) s[i] += sp.s[i]; return *this; }
  Spectrum& operator*=(const Spectrum& sp) { for (int i = 0; i < nrSpectrumSamples; i++) s[i] *= sp.s[i]; return *this; }
  Spectrum& operator*=(const Float f) { assert(f >= 0); for (int i = 0; i < nrSpectrumSamples; i++) s[i] *= f; return *this; }
  Spectrum& operator/=(const Float f) { assert(f > 0); for (int i = 0; i < nrSpectrumSamples; i++) s[i] /= f; return *this; }
  Float NormSquared() const { Float sum = 0; for (int i = 0; i < nrSpectrumSamples; i++) sum += s[i]*s[i]; return sum; }
  Float Norm() const { return std::sqrt(NormSquared()); }
};

inline Spectrum operator+(const Spectrum& a, const Spectrum& b) {
  Spectrum c;
  for (int i = 0; i < nrSpectrumSamples; i++)
    c.s[i] = a.s[i] + b.s[i];
  return c;
}

inline Spectrum operator*(const Spectrum& a, const Spectrum& b) {
  Spectrum c;
  for (int i = 0; i < nrSpectrumSamples; i++)
    c.s[i] = a.s[i] * b.s[i];
  return c;
}

inline Spectrum operator*(const Spectrum& a, const Float f) {
  assert(f >= 0);
  Spectrum c;
  for (int i = 0; i < nrSpectrumSamples; i++)
    c.s[i] = a.s[i] * f;
  return c;
}

inline Spectrum operator*(const Float f, const Spectrum& a) {
  Spectrum c;
  for (int i = 0; i < nrSpectrumSamples; i++)
    c.s[i] = a.s[i] * f;
  return c;
}

inline Spectrum operator/(const Spectrum& a, const Float f) {
  assert(f > 0);
  Spectrum c;
  for (int i = 0; i < nrSpectrumSamples; i++)
    c.s[i] = a.s[i] / f;
  return c;
}


}
#endif
