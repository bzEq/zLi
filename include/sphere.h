#ifndef _ZLI_SPHERE_H_
#define _ZLI_SPHERE_H_
#include "geometry.h"
#include "ray.h"
#include "shape.h"
#include "boundbox.h"

#include <boost/optional.hpp>
#include <iostream>


namespace zLi {
struct Sphere: public Shape {
  Vector3f c;
  Float r;
  Sphere(): r(1) {}
  Sphere(const Vector3f& c, const Float r): c(c), r(r) {}
  ~Sphere() {}
  boost::optional<RayIntersection> Intersect(const Ray& ray) const {
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
    return RayIntersection{ .t = t, .shape = this, .ray = ray };
  }
  BoundBox Bounds() const {
    return BoundBox(Vector3f(c.x-r, c.y-r, c.z-r),
                    Vector3f(c.x+r, c.y+r, c.z+r));
  }
};

} // end namespace zLi
#endif
