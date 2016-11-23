#ifndef _ZLI_TRIANGLE_HH_
#define _ZLI_TRIANGLE_HH_
#include "boundbox.hh"
#include "bsdf.hh"
#include "geometry.hh"
#include "math.hh"
#include "ray.hh"
#include "spectrum.hh"

#include <iostream>
#include <memory>

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
  BSDF &bsdf() { return bsdf_; }
  Vector3f Normal(const Vector3f &_);
  Geometry ImplGeometry();
};

} // end namespace zLi

#endif
