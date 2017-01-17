// Copyright (c) 2016 Kai Luo. All rights reserved.

#include <functional>

#include "triangle.h"

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

BoundBox Triangle::Bounds() {
  return BoundBox(Vector3f(std::min(a.x, std::min(b.x, c.x)) - EPSILON,
                           std::min(a.y, std::min(b.y, c.y)) - EPSILON,
                           std::min(a.z, std::min(b.z, c.z)) - EPSILON),
                  Vector3f(std::max(a.x, std::max(b.x, c.x)) + EPSILON,
                           std::max(a.y, std::max(b.y, c.y)) + EPSILON,
                           std::max(a.z, std::max(b.z, c.z)) + EPSILON));
}

Vector3f Triangle::Normal(const Vector3f &position) { return n; }

Geometry Triangle::ImplGeometry() {
  Geometry g;
  g.Intersect = std::bind(&Triangle::Intersect, shared_from_this(),
                          std::placeholders::_1);
  g.Bounds = std::bind(&Triangle::Bounds, shared_from_this());
  g.Normal =
      std::bind(&Triangle::Normal, shared_from_this(), std::placeholders::_1);
  g.Le = std::bind(&Triangle::Le, shared_from_this());
  g.R = std::bind(&Triangle::R, shared_from_this());
  g.bsdf = std::bind(&Triangle::bsdf, shared_from_this());
  return g;
}

}  // namespace zLi
