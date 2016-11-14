#ifndef _ZLI_SPHERE_HH_
#define _ZLI_SPHERE_HH_
#include "boundbox.hh"
#include "geometry.hh"
#include "math.hh"
#include "ray.hh"

#include <iostream>
#include <memory>

namespace zLi {
struct Sphere : std::enable_shared_from_this<Sphere> {
  Vector3f c;
  Float r;
  Spectrum le;
  Spectrum rfl;
  BSDF bsdf_;
  Sphere() : r(1) {}
  Sphere(const Vector3f &c, Float r) : c(c), r(r) {}
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

} // end namespace zLi
#endif
