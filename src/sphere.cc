#include "sphere.hh"
#include "logging.hh"

namespace zLi {

std::optional<RaySurfaceIntersection> Sphere::Intersect(const Ray &ray) {
  auto d = ray.o - c;
  auto a = ray.d * ray.d;
  auto b = 2 * (d * ray.d);
  auto f = d * d - r * r;
  auto res = Quadratic(a, b, f);
  if (!res) {
    return {};
  }
  auto t0 = std::get<0>(*res);
  auto t1 = std::get<1>(*res);
  assert(t0 <= t1);
  if (t0 < ray.tmin && t1 < ray.tmin) {
    return {};
  }
  auto t = t0 < ray.tmin ? t1 : t0;
  assert(t >= ray.tmin);
  return RaySurfaceIntersection{
      .t = t, .ray = ray, .g = ImplGeometry(),
  };
}

BoundBox Sphere::Bounds() {
  return BoundBox(Vector3f(c.x - r, c.y - r, c.z - r),
                  Vector3f(c.x + r, c.y + r, c.z + r));
}

Geometry Sphere::ImplGeometry() {
  return Geometry{
      .Intersect = std::bind(&Sphere::Intersect, shared_from_this(),
                             std::placeholders::_1),
      .Bounds = std::bind(&Sphere::Bounds, shared_from_this()),
      .Normal =
          std::bind(&Sphere::Normal, shared_from_this(), std::placeholders::_1),
      .Le = std::bind(&Sphere::Le, shared_from_this()),
      .R = std::bind(&Sphere::R, shared_from_this()),
      .bsdf = std::bind(&Sphere::bsdf, shared_from_this()),
  };
}

} // namespace zLi
