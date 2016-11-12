#ifndef _ZLI_SCENE_HH_
#define _ZLI_SCENE_HH_
#include "geometry.hh"
#include "kdtree.hh"
#include "math.hh"
#include "result.hh"
#include "spectrum.hh"

#include <string>

namespace zLi {

class Scene {
public:
  static Result<Scene> SceneFromJson(const std::string &file);
  Spectrum DirectLight(const Vector3f &position) const;
  Spectrum DirectLight(const RaySurfaceIntersection &ri) const;
  std::optional<RaySurfaceIntersection> Intersect(const Ray &) const;

private:
  Scene() = default;
  // KdTree kdt_;
};

} // end namespace zLi
#endif
