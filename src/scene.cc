#include "scene.hh"
#include "geometry.hh"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <string>

namespace zLi {

Result<Scene> Scene::SceneFromJson(const std::string &file) {
  boost::property_tree::ptree json;
  try {
    boost::property_tree::read_json(file, json);
    // read geometries
    std::vector<Geometry> gs;
    Scene scene((KdTree(gs)));
    // add lights
    return Ok(std::move(scene));
  } catch (std::exception &e) {
    return Error(e.what());
  }
}

Spectrum Scene::DirectLight(const RaySurfaceIntersection &ri) const {
  for (auto &l : lights_) {
  }
}

std::optional<RaySurfaceIntersection> Scene::Intersect(const Ray &ray) const {
  return kdt_.Intersect(ray);
}

bool Scene::IsVisible(const RaySurfaceIntersection &ri, const Vector3f &p) {
  auto ray = ri.SpawnRayTowards(p);
  auto a = ri.ray(ri.t);
  auto test = Intersect(ray);
  if (!test) {
    return false;
  }
  Float t = (p - a)[0] / ray.d[0];
  return t <= (*test).t;
}

} // zLi
