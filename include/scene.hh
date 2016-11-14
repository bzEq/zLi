#ifndef _ZLI_SCENE_HH_
#define _ZLI_SCENE_HH_
#include "geometry.hh"
#include "kdtree.hh"
#include "light.hh"
#include "math.hh"
#include "result.hh"
#include "spectrum.hh"

#include <string>
#include <vector>

namespace zLi {

class Scene {
public:
  Scene(Scene &&) = default;
  static Result<Scene> SceneFromJson(const std::string &file);
  Spectrum DirectLight(const RaySurfaceIntersection &ri) const;
  bool IsVisible(const RaySurfaceIntersection &, const Vector3f &);
  std::optional<RaySurfaceIntersection> Intersect(const Ray &) const;

private:
  Scene() = default;
  Scene(KdTree &&kdt) : kdt_(std::move(kdt)) {}
  std::vector<Light> lights_;
  KdTree kdt_;
};

} // end namespace zLi
#endif
