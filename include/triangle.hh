#ifndef _ZLI_TRIANGLE_H_
#define _ZLI_TRIANGLE_H_
#include "geometry.hh"
#include "ray.hh"
#include "shape.hh"
#include "boundbox.hh"
#include "spectrum.hh"
#include "bsdf.hh"

#include <iostream>
#include <memory>


namespace zLi {
struct Triangle: public Shape, std::enable_shared_from_this<Shape> {
  Vector3f a, b, c, n;
  Spectrum le;
  std::shared_ptr<BSDF> bsdf_;
  Triangle() {}
  // counter clockwise
  Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c,
           const Vector3f& n)
    : a(a), b(b), c(c), n(n) {}
  Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c):
    a(a), b(b), c(c), n(((b-a)^(c-a)).Normalize()) {}
  ~Triangle() {}
  std::optional<RayIntersection> Intersect(const Ray& ray) {
    auto e = ray.o - a;
    auto ba = b-a;
    auto ca = c-a;
    auto det = Determinant3x3(ba, ca, -ray.d);
    if (std::abs(det) < EPSILON) return {};
    auto beta = Determinant3x3(e, ca, -ray.d) / det;
    if (beta < 0 || beta > 1) return {};
    auto gamma = Determinant3x3(ba, e, -ray.d) / det;
    if (gamma < 0 || gamma > 1 - beta) return {};
    auto t = Determinant3x3(ba, ca, e) / det;
    if (t < ray.tmin || t > ray.tmax) return {};
    return RayIntersection{ .t = t, .shape = shared_from_this(), .ray = ray };
  }
  BoundBox Bounds() {
    return BoundBox(Vector3f(std::min(a.x, std::min(b.x, c.x)),
                             std::min(a.y, std::min(b.y, c.y)),
                             std::min(a.z, std::min(b.z, c.z))),
                    Vector3f(std::max(a.x, std::max(b.x, c.x)),
                             std::max(a.y, std::max(b.y, c.y)),
                             std::max(a.z, std::max(b.z, c.z))));
  }
  Spectrum Le() {
    return le;
  }
  std::shared_ptr<BSDF> bsdf() {
    return bsdf_;
  }
  Vector3f Normal(const Vector3f& _) {
    return n;
  }
};

//struct Mesh: public Shape, std::enable_shared_from_this<Shape> {
//  std::vector<Vector3f> vertices;
//  std::vector<std::tuple<unsigned, unsigned, unsigned> > triangles;
//  Mesh() {}
//  Mesh(const std::vector<Vector3f>& vertices,
//       const std::vector<std::tuple<unsigned, unsigned, unsigned>>& triangles)
//    : vertices(vertices), triangles(triangles) {}
//  ~Mesh() {}
//  std::optional<RayIntersection> Intersect(const Ray& ray) const {
//    for (auto tri: triangles) {
//      unsigned index[3] = { std::get<0>(tri), std::get<1>(tri), std::get<2>(tri) };
//      assert(index[0] < vertices.size() && index[0] >= 0);
//      assert(index[1] < vertices.size() && index[1] >= 0);
//      assert(index[2] < vertices.size() && index[2] >= 0);
//      auto t = std::make_shared<Triangle>(vertices[index[0]], vertices[index[1]], vertices[index[2]]);
//      auto ri = t->Intersect(ray);
//      if (ri) return ri;
//    }
//    return {};
//  }
//  bool IsManifold() const;
//  BoundBox Bounds() const;
//};

} // end namespace zLi

#endif
