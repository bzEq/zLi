#ifndef _ZLI_SAMPLE_HH_
#define _ZLI_SAMPLE_HH_
#include "math.hh"

#include <cstdlib>
#include <random>
#include <tuple>

namespace zLi {

inline Float UniformSample() {
  static std::random_device rd;
  static std::mt19937 g(rd());
  static std::uniform_real_distribution<Float> u(0, 1);
  return u(g);
}

inline Vector3f SampleFromHemiSphere() {
  Float u = 2 * PI * UniformSample();
  Float v = UniformSample();
  return Vector3f(std::cos(u) * std::sqrt(v), std::sin(u) * std::sqrt(v),
                  std::sqrt(1 - v));
}

inline Vector3f SampleFromSphere() {
  Vector3f v = SampleFromHemiSphere();
  return UniformSample() <= 0.5 ? v : -v;
}

inline std::tuple<Float, Float> SampleFromDisk() {
  Float r = UniformSample();
  Float u = 2 * PI * UniformSample();
  return std::make_tuple(r * std::cos(u), r * std::sin(u));
}

inline std::tuple<Float, Float> SampleFromCircle() {
  Float u = 2 * PI * UniformSample();
  return std::make_tuple(std::cos(u), std::sin(u));
}

} // end namespace zLi

#endif
