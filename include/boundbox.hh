#ifndef _ZLI_BOUNDBOX_HH_
#define _ZLI_BOUNDBOX_HH_
#include "math.hh"
#include "ray.hh"

#include <iostream>

namespace zLi {

struct BoundBox {
  Vector3f pMin, pMax; // pMin for bottom left corner, pMax for top right corner
  BoundBox()
      : pMin(Vector3f(INF, INF, INF)), pMax(Vector3f(NINF, NINF, NINF)) {}
  BoundBox(const Vector3f &p, const Vector3f &q)
      : pMin(std::min(p.x, q.x), std::min(p.y, q.y), std::min(p.z, q.z)),
        pMax(std::max(p.x, q.x), std::max(p.y, q.y), std::max(p.z, q.z)) {}
  Vector3f Diagonal() const { return pMax - pMin; }
  Float SurfaceArea() const {
    Vector3f d = Diagonal();
    return 2 * (d.x * d.y + d.y * d.z + d.z * d.x);
  }
  Float Volume() const {
    Vector3f d = Diagonal();
    return d.x * d.y * d.z;
  }
  bool InBox(const Vector3f &p) const;
  Vector3f Middle() const { return (pMin + pMax) * (Float)0.5; }
  bool CanContain(const BoundBox &b) const {
    for (int i = 0; i < 3; ++i) {
      if (b.pMin[i] < pMin[i] || b.pMax[i] > pMax[i]) {
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
