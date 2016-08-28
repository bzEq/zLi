#ifndef _ZLI_TRIANGLE_H_
#define _ZLI_TRIANGLE_H_
#include "geometry.h"
#include "ray.h"
#include "shape.h"

#include <boost/optional.hpp>
#include <iostream>


namespace zLi {
struct Triangle: public Shape {
  Vector3f a, b, c, n;
  Triangle() {}
  // counter clockwise
  Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c, 
           const Vector3f& n)
    : a(a), b(b), c(c), n(n) {}
  Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c):
    a(a), b(b), c(c), n(((b-a)^(c-a)).Normalize()) {}
  ~Triangle() {}
  boost::optional<RayIntersection> Intersect(const Ray& ray) const {
    auto e = ray.o - a;
    auto ba = b-a;
    auto ca = c-a;
    auto det = Determinant3x3(ba, ca, -ray.d);
    if (det == 0) return {};
    auto beta = Determinant3x3(e, ca, -ray.d) / det;
    if (beta < 0 || beta > 1) return {};
    auto gamma = Determinant3x3(ba, e, -ray.d) / det;
    if (gamma < 0 || gamma > 1 - beta) return {};
    auto t = Determinant3x3(ba, ca, e) / det;
    if (t < ray.tmin || t > ray.tmax) return {};
    return RayIntersection{ .t = t, .shape = this };
  }
};

} // end namespace zLi

#endif