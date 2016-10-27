#ifndef _ZLI_TRIANGLE_HH_
#define _ZLI_TRIANGLE_HH_
#include "boundbox.hh"
#include "geometry.hh"
#include "math.hh"
#include "ray.hh"
#include "spectrum.hh"

#include <iostream>
#include <memory>

namespace zLi {
struct Triangle : std::enable_shared_from_this<Triangle> {
  Vector3f a, b, c, n;
  Spectrum le;
  Triangle() {}
  // counter clockwise
  Triangle(const Vector3f &a, const Vector3f &b, const Vector3f &c,
           const Vector3f &n)
      : a(a), b(b), c(c), n(n) {}

  Triangle(const Vector3f &a, const Vector3f &b, const Vector3f &c)
      : a(a), b(b), c(c), n(((b - a) ^ (c - a)).Normalize()) {}

  ~Triangle() {}

  std::optional<RaySurfaceIntersection> Intersect(const Ray &ray);

  BoundBox Bounds() {
    return BoundBox(Vector3f(std::min(a.x, std::min(b.x, c.x)),
                             std::min(a.y, std::min(b.y, c.y)),
                             std::min(a.z, std::min(b.z, c.z))),
                    Vector3f(std::max(a.x, std::max(b.x, c.x)),
                             std::max(a.y, std::max(b.y, c.y)),
                             std::max(a.z, std::max(b.z, c.z))));
  }
  Spectrum Le() { return le; }
  Vector3f Normal(const Vector3f &_) { return n; }
  Geometry ImplGeometry();
};

} // end namespace zLi

#endif
