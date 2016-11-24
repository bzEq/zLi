// Copyright (c) 2016 Kai Luo. All rights reserved.

#include "boundbox.hh"

namespace zLi {

std::optional<std::tuple<Float, Float>>
BoundBox::Intersect(const Ray &ray) const {
  Float t0 = ray.tmin, t1 = ray.tmax;
  for (int i = 0; i < 3; i++) {
    if (ray.d[i] != 0) {
      Float tmp0 = (min_point[i] - ray.o[i]) / ray.d[i];
      Float tmp1 = (max_point[i] - ray.o[i]) / ray.d[i];
      t0 = std::max(t0, std::min(tmp0, tmp1));
      t1 = std::min(t1, std::max(tmp0, tmp1));
    }
  }
  if (t0 > t1 || (!InBox(ray(t0)) && !InBox(ray(t1)))) {
    return {};
  }
  return std::make_tuple(t0, t1);
}

std::optional<std::tuple<BoundBox, BoundBox>> BoundBox::Split(int axis,
                                                              Float d) const {
  if (d < min_point[axis] || d > max_point[axis])
    return {};
  Vector3f v0(min_point);
  v0[axis] = d;
  Vector3f v1(max_point);
  v1[axis] = d;
  return std::make_tuple(BoundBox(min_point, v1), BoundBox(v0, max_point));
}

bool BoundBox::InBox(const Vector3f &p) const {
  for (int i = 0; i < 3; i++) {
    if (p[i] < min_point[i] || p[i] > max_point[i])
      return false;
  }
  return true;
}

BoundBox Union(const BoundBox &lhs, const BoundBox &rhs) {
  BoundBox res;
  for (int i = 0; i < 3; ++i) {
    res.min_point[i] = std::min(lhs.min_point[i], rhs.min_point[i]);
    res.max_point[i] = std::max(lhs.max_point[i], rhs.max_point[i]);
  }
  return res;
}

}  // namespace zLi
