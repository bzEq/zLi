#ifndef _ZLI_SCENE_HH_
#define _ZLI_SCENE_HH_
#include "bsdf.hh"
#include "camera.hh"
#include "geometry.hh"
#include "kdtree.hh"
#include "light.hh"
#include "math.hh"
#include "ray.hh"
#include "result.hh"
#include "spectrum.hh"
#include "sphere.hh"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <string>
#include <vector>

namespace zLi {

class Scene {
public:
  Scene(Scene &&) = default;
  static Result<Scene> SceneFromJson(const std::string &file);
  static Result<Sphere> SphereFromJson(const boost::property_tree::ptree &);
  static std::vector<Geometry>
  GeometriesFromJson(const boost::property_tree::ptree &);
  static PerspectiveCamera CameraFromJson(const boost::property_tree::ptree &);
  Spectrum DirectLight(const RaySurfaceIntersection &ri) const;
  bool IsVisible(const RaySurfaceIntersection &, const Vector3f &);
  std::optional<RaySurfaceIntersection> Intersect(const Ray &) const;
  Ray GenerateRay(Float, Float);

private:
  Scene() = default;
  Scene(KdTree &&kdt) : kdt_(std::move(kdt)) {}
  Scene(std::vector<Geometry> &&gs) : gs_(std::move(gs)) {}
  std::vector<Light> lights_;
  PerspectiveCamera camera_;
  std::vector<Geometry> gs_;
  KdTree kdt_;
};

} // end namespace zLi
#endif
