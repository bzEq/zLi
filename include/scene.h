// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef _ZLI_SCENE_H_
#define _ZLI_SCENE_H_
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <memory>
#include <string>
#include <vector>

#include "bsdf.h"
#include "camera.h"
#include "geometry.h"
#include "kdtree.h"
#include "light.h"
#include "core_math.h"
#include "ray.h"
#include "result.h"
#include "spectrum.h"
#include "sphere.h"
#include "triangle.h"

namespace zLi {

class Scene {
public:
  Scene(Scene &&) = default;
  static Result<Scene> SceneFromJson(const std::string &file);
  static Result<Sphere> SphereFromJson(const boost::property_tree::ptree &);
  static Result<PointLight>
  PointLightFromJson(const boost::property_tree::ptree &);
  static Result<Triangle> TriangleFromJson(const boost::property_tree::ptree &);
  static std::vector<Geometry>
  GeometriesFromJson(const boost::property_tree::ptree &);
  static std::vector<Light> LightsFromJson(const boost::property_tree::ptree &);
  static PerspectiveCamera CameraFromJson(const boost::property_tree::ptree &);
  Spectrum DirectLight(const RaySurfaceIntersection &) const;
  bool IsVisible(const RaySurfaceIntersection &, const Vector3f &);
  std::optional<RaySurfaceIntersection> Intersect(const Ray &) const;
  std::optional<RaySurfaceIntersection> NaiveIntersect(const Ray &) const;
  Ray GenerateRay(Float, Float);

private:
  Scene() = default;
  explicit Scene(std::vector<Geometry> &&gs) : gs_(gs) {}  // kdt_(gs_) {}
  std::vector<Light> lights_;
  PerspectiveCamera camera_;
  std::vector<Geometry> gs_;
  std::unique_ptr<KdTree> kdt_;
};

}  // end namespace zLi
#endif
