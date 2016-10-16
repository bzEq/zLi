#ifndef _ZLI_SPHERE_H_
#define _ZLI_SPHERE_H_
#include "geometry.hh"
#include "ray.hh"
#include "shape.hh"
#include "boundbox.hh"

#include <iostream>
#include <memory>


namespace zLi {
struct Sphere: public Shape, std::enable_shared_from_this<Shape> {
  Vector3f c;
  Float r;
  Spectrum le;
  std::shared_ptr<BSDF> bsdf_;
  Sphere(): r(1) {}
  Sphere(const Vector3f& c, const Float r): c(c), r(r) {}
  ~Sphere() {}
  std::optional<RayIntersection> Intersect(const Ray& ray) const {
    auto d = ray.o - c;
    auto a = ray.d*ray.d;
    auto b = 2 * (d * ray.d);
    auto c = d*d - r*r;
    auto res = Quadratic(a, b, c);
    if (!res) return {};
    auto t0 = std::get<0>(*res);
    auto t1 = std::get<1>(*res);
    assert(t0<=t1);
    if (t0 < ray.tmin && t1 < ray.tmin) return {};
    auto t = t0 < ray.tmin ? t1: t0;
    assert(t >= ray.tmin);
    return RayIntersection{ .t = t, .shape = shared_from_this(), .ray = ray };
  }
  BoundBox Bounds() const {
    return BoundBox(Vector3f(c.x-r, c.y-r, c.z-r),
                    Vector3f(c.x+r, c.y+r, c.z+r));
  }
  Spectrum Le() const {
    return le;
  }
  std::shared_ptr<const BSDF> bsdf() const {
    return bsdf_;
  }
  Vector3f Normal(const Vector3f& position) const {
    return (position-c).Normalize();
  }
};

} // end namespace zLi
#endif
