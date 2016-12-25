// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef _ZLI_TRIANGLE_H_
#define _ZLI_TRIANGLE_H_
#include <iostream>
#include <memory>
#include <utility>

#include "boundbox.h"
#include "bsdf.h"
#include "geometry.h"
#include "core_math.h"
#include "ray.h"
#include "spectrum.h"

namespace zLi {
struct Triangle : std::enable_shared_from_this<Triangle> {
  Vector3f a, b, c, n;
  Spectrum le, rfl;
  BSDF bsdf_;
  Triangle() {}
  // counter clockwise
  Triangle(const Vector3f &a, const Vector3f &b, const Vector3f &c,
           const Vector3f &n)
      : a(a), b(b), c(c), n(n) {}

  Triangle(const Vector3f &a, const Vector3f &b, const Vector3f &c,
           const Spectrum &le = Spectrum(), const Spectrum &rfl = Spectrum(),
           BSDF &&bsdf = LambertianDiffuse::ImplBSDF())
      : a(a), b(b), c(c), n(((b - a) ^ (c - a)).Normalize()), le(le), rfl(rfl),
        bsdf_(std::move(bsdf)) {}

  ~Triangle() {}

  std::optional<RaySurfaceIntersection> Intersect(const Ray &ray);

  BoundBox Bounds();
  Spectrum R() { return rfl; }
  Spectrum Le() { return le; }
  BSDF bsdf() { return bsdf_; }
  Vector3f Normal(const Vector3f &_);
  Geometry ImplGeometry();
};

}  // end namespace zLi

#endif
