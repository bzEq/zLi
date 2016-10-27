#include "boundbox.hh"

namespace zLi {

std::optional<std::tuple<Float, Float>>
BoundBox::Intersect(const Ray &ray) const {
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
  if (t0 > t1 || !InBox(ray(t0)) || !InBox(ray(t1)))
    return {};
  return std::make_tuple(t0, t1);
}

std::optional<std::tuple<BoundBox, BoundBox>> BoundBox::Split(int axis,
                                                              Float d) const {
  if (d <= pMin[axis] || d >= pMax[axis])
    return {};
  Vector3f v0(pMin);
  v0[axis] = d;
  Vector3f v1(pMax);
  v1[axis] = d;
  return std::make_tuple(BoundBox(pMin, v1), BoundBox(v0, pMax));
}

} // namespace zLi
