// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef _ZLI_SPHERE_HH_
#define _ZLI_SPHERE_HH_
#include <iostream>
#include <memory>
#include <utility>

#include "boundbox.hh"
#include "bsdf.hh"
#include "geometry.hh"
#include "math.hh"
#include "ray.hh"

namespace zLi {
struct Sphere : std::enable_shared_from_this<Sphere> {
  Vector3f c;
  Float r;
  Spectrum le;
  Spectrum rfl;
  BSDF bsdf_;
  Sphere() : r(1) {}
  Sphere(const Vector3f &c, Float r, const Spectrum &le = Spectrum(),
         const Spectrum &rfl = Spectrum(),
         BSDF &&bsdf = LambertianDiffuse::ImplBSDF())
      : c(c), r(r), le(le), rfl(rfl), bsdf_(std::move(bsdf)) {}
  ~Sphere() {}
  std::optional<RaySurfaceIntersection> Intersect(const Ray &ray);
  BoundBox Bounds();
  Spectrum Le() { return le; }
  Spectrum R() { return rfl; }
  BSDF bsdf() { return bsdf_; }
  Vector3f Normal(const Vector3f &position) {
    return (position - c).Normalize();
  }
  Geometry ImplGeometry();
};

}  // end namespace zLi
#endif
