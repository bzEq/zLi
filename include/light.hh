// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef _ZLI_LIGHT_HH_
#define _ZLI_LIGHT_HH_
#include <functional>
#include <memory>

#include "geometry.hh"
#include "math.hh"
#include "spectrum.hh"

namespace zLi {

struct Light {
  std::function<Spectrum(const RaySurfaceIntersection &)> Le;
};

class PointLight : public std::enable_shared_from_this<PointLight> {
public:
  PointLight(PointLight &&) = default;
  PointLight(const Vector3f &p, const Spectrum &s) : position_(p), le_(s) {}
  Spectrum Le(const RaySurfaceIntersection &);
  Light ImplLight();

private:
  Vector3f position_;
  Spectrum le_;
};

}  // namespace zLi

#endif
