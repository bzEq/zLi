#ifndef _ZLI_BOUNDBOX_H_
#define _ZLI_BOUNDBOX_H_
#include "geometry.hh"
#include "ray.hh"

#include <iostream>


namespace zLi {

struct BoundBox {
  Vector3f pMin, pMax; // pMin for bottom left corner, pMax for top right corner
  BoundBox(): pMin(Vector3f(INF, INF, INF)),
              pMax(Vector3f(NINF, NINF, NINF)) {}
  BoundBox(const Vector3f& p, const Vector3f& q)
    : pMin(std::min(p.x, q.x),
           std::min(p.y, q.y),
           std::min(p.z, q.z)),
      pMax(std::max(p.x, q.x),
           std::max(p.y, q.y),
           std::max(p.z, q.z)) {}
  Vector3f Diagonal() const { return pMax - pMin; }
  Float SurfaceArea() const {
    Vector3f d = Diagonal();
    return 2 * (d.x * d.y + d.y * d.z + d.z + d.x);
  }
  Float Volume() const {
    Vector3f d = Diagonal();
    return d.x * d.y * d.z;
  }
  bool InBox(const Vector3f& p) const {
    for (int i = 0; i < 3; i++) {
      if (p[i] < pMin[i] || p[i] > pMax[i]) return false;
    }
    return true;
  }
  std::optional< std::tuple<Float, Float> > Intersect(const Ray& ray) const {
    Float t0 = ray.tmin, t1 = ray.tmax;
    for (int i = 0; i < 3; i++) {
      if (ray.d[i] != 0) {
        Float d0 = pMin[i];
        Float d1 = pMax[i];
        Float tmp0 = (d0 - ray.o[i]) / ray.d[i];
        Float tmp1 = (d1 - ray.o[i]) / ray.d[i];
        t0 = std::max(t0, std::min(tmp0, tmp1));
        t1 = std::min(t1, std::max(tmp0, tmp1));
      }
    }
    if (t0 > t1 || !InBox(ray(t0)) || !InBox(ray(t1))) return {};
    return std::make_tuple(t0, t1);
  }
  std::optional< std::tuple<BoundBox, BoundBox> >
  Split(int axis, Float d) const {
    if (d <= pMin[axis] || d >= pMax[axis]) return {};
    Vector3f v0(pMin);
    v0[axis] = d;
    Vector3f v1(pMax);
    v1[axis] = d;
    return std::make_tuple(BoundBox(pMin, v1),
                           BoundBox(v0, pMax));
  }
};

}
#endif
