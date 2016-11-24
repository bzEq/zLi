#ifndef _ZLI_BOUNDBOX_HH_
#define _ZLI_BOUNDBOX_HH_
#include "math.hh"
#include "ray.hh"

#include <iostream>

namespace zLi {

struct BoundBox {
  Vector3f min_point, max_point; // min_point for bottom left corner, max_point
                                 // for top right corner
  BoundBox()
      : min_point(Vector3f(INF, INF, INF)),
        max_point(Vector3f(NINF, NINF, NINF)) {}
  BoundBox(const Vector3f &p, const Vector3f &q)
      : min_point(std::min(p.x, q.x), std::min(p.y, q.y), std::min(p.z, q.z)),
        max_point(std::max(p.x, q.x), std::max(p.y, q.y), std::max(p.z, q.z)) {}
  Vector3f Diagonal() const { return max_point - min_point; }
  Float SurfaceArea() const {
    Vector3f d = Diagonal();
    return 2 * (d.x * d.y + d.y * d.z + d.z * d.x);
  }
  Float Volume() const {
    Vector3f d = Diagonal();
    return d.x * d.y * d.z;
  }
  bool InBox(const Vector3f &p) const;
  Vector3f Middle() const { return (min_point + max_point) * Float{0.5}; }
  bool CanContain(const BoundBox &b) const {
    for (int i = 0; i < 3; ++i) {
      if (b.min_point[i] < min_point[i] || b.max_point[i] > max_point[i]) {
        return false;
      }
    }
    return true;
  }
  friend BoundBox Union(const BoundBox &lhs, const BoundBox &rhs);
  std::optional<std::tuple<Float, Float>> Intersect(const Ray &ray) const;
  std::optional<std::tuple<BoundBox, BoundBox>> Split(int axis, Float d) const;
};
}
#endif
