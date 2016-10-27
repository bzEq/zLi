#include "triangle.hh"

#include <functional>

namespace zLi {

std::optional<RaySurfaceIntersection> Triangle::Intersect(const Ray &ray) {
  auto e = ray.o - a;
  auto ba = b - a;
  auto ca = c - a;
  auto det = Determinant3x3(ba, ca, -ray.d);
  if (std::abs(det) < EPSILON)
    return {};
  auto beta = Determinant3x3(e, ca, -ray.d) / det;
  if (beta < 0 || beta > 1)
    return {};
  auto gamma = Determinant3x3(ba, e, -ray.d) / det;
  if (gamma < 0 || gamma > 1 - beta)
    return {};
  auto t = Determinant3x3(ba, ca, e) / det;
  if (t < ray.tmin || t > ray.tmax)
    return {};
  return RaySurfaceIntersection{
      .t = t, .ray = ray, .g = ImplGeometry(),
  };
}

Geometry Triangle::ImplGeometry() {
  return Geometry{
      .Intersect = std::bind(&Triangle::Intersect, shared_from_this(),
                             std::placeholders::_1),
  };
}

} // namespace zLi
