#include "sphere.hh"

namespace zLi {

std::optional<RaySurfaceIntersection> Sphere::Intersect(const Ray &ray) {
  auto d = ray.o - c;
  auto a = ray.d * ray.d;
  auto b = 2 * (d * ray.d);
  auto c = d * d - r * r;
  auto res = Quadratic(a, b, c);
  if (!res)
    return {};
  auto t0 = std::get<0>(*res);
  auto t1 = std::get<1>(*res);
  assert(t0 <= t1);
  if (t0 < ray.tmin && t1 < ray.tmin)
    return {};
  auto t = t0 < ray.tmin ? t1 : t0;
  assert(t >= ray.tmin);
  return RaySurfaceIntersection{
      .t = t, .ray = ray, .g = ImplGeometry(),
  };
}

Geometry Sphere::ImplGeometry() {
  return Geometry{
      .Intersect = std::bind(&Sphere::Intersect, shared_from_this(),
                             std::placeholders::_1),
  };
}

} // namespace zLi
