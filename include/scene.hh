#ifndef _ZLI_SCENE_H_
#define _ZLI_SCENE_H_
#include "spectrum.hh"
#include "shape.hh"


namespace zLi {

struct Scene {
  Spectrum DirectLight(const Vector3f& position) const;
  Spectrum DirectLight(const RayIntersection& ri) const;
  std::optional<RayIntersection> Intersect(const Ray&) const;
};

} // end namespace zLi
#endif
