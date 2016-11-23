#include "scene.hh"
#include "geometry.hh"
#include "logging.hh"
#include "sphere.hh"
#include "triangle.hh"
#include "utils.hh"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <string>

namespace zLi {

std::vector<Geometry>
Scene::GeometriesFromJson(const boost::property_tree::ptree &json) {
  std::vector<Geometry> gs;
  for (auto it = json.begin(); it != json.end(); ++it) {
    auto s = it->second;
    auto type = s.get<std::string>("type");
    DEBUG("geometry type: %s", type.c_str());
    if (type == "sphere") {
      auto res = SphereFromJson(s);
      if (res) {
        gs.push_back(std::make_shared<Sphere>(std::move(*res))->ImplGeometry());
      } else {
        WARN(res.Error().c_str());
      }
    } else if (type == "triangle") {
      auto res = TriangleFromJson(s);
      if (res) {
        gs.push_back(
            std::make_shared<Triangle>(std::move(*res))->ImplGeometry());
      } else {
        WARN(res.Error().c_str());
      }
    }
  }
  return gs;
}

PerspectiveCamera
Scene::CameraFromJson(const boost::property_tree::ptree &json) {
  auto eye = Utils::Vector3FromJson(json.get_child("eye"));
  auto lookat = Utils::Vector3FromJson(json.get_child("lookat"));
  auto up = Utils::Vector3FromJson(json.get_child("up"));
  auto fov = json.get<Float>("fov");
  auto lensr = json.get<Float>("lensr");
  auto lensp = json.get<Float>("lensp");
  return PerspectiveCamera(eye, lookat, up, fov, lensr, lensp);
}

Result<Triangle>
Scene::TriangleFromJson(const boost::property_tree::ptree &json) {
  try {
    Vector3f a = Utils::Vector3FromJson(json.get_child("a"));
    Vector3f b = Utils::Vector3FromJson(json.get_child("b"));
    Vector3f c = Utils::Vector3FromJson(json.get_child("c"));
    Spectrum Le(Utils::SpectrumFromJson(json.get_child("le")));
    Spectrum R(Utils::SpectrumFromJson(json.get_child("R")));
    return Ok(
        Triangle(a, b, c, Le, R, Utils::BSDFFromJson(json.get_child("bsdf"))));
  } catch (const std::exception &e) {
    return ::Error(e.what());
  }
}

Result<Scene> Scene::SceneFromJson(const std::string &file) {
  boost::property_tree::ptree json;
  try {
    boost::property_tree::read_json(file, json);
    // read geometries
    std::vector<Geometry> gs(GeometriesFromJson(json.get_child("geometry")));
    // Scene scene((KdTree(gs)));
    auto rep(gs);
    Scene scene(std::move(gs));
    scene.kdt_ = std::make_unique<KdTree>(KdTree::BuildKdTree(std::move(rep)));
    // add lights
    // add camera
    scene.camera_ = CameraFromJson(json.get_child("camera"));
    return Ok(std::move(scene));
  } catch (std::exception &e) {
    return ::Error(e.what());
  }
}

Ray Scene::GenerateRay(Float u, Float v) { return camera_.GenerateRay(u, v); }

Result<Sphere> Scene::SphereFromJson(const boost::property_tree::ptree &json) {
  try {
    Float radius = json.get<Float>("radius");
    Vector3f center(Utils::Vector3FromJson(json.get_child("center")));
    Spectrum Le(Utils::SpectrumFromJson(json.get_child("le")));
    Spectrum R(Utils::SpectrumFromJson(json.get_child("R")));
    return Ok(Sphere(center, radius, Le, R,
                     Utils::BSDFFromJson(json.get_child("bsdf"))));
  } catch (const std::exception &e) {
    return ::Error(e.what());
  }
}

Spectrum Scene::DirectLight(const RaySurfaceIntersection &ri) const {
  return ri.g.Le();
}

std::optional<RaySurfaceIntersection> Scene::Intersect(const Ray &ray) const {
  // if (kdt_) {
  //   return kdt_->Intersect(ray);
  // }
  return NaiveIntersect(ray);
}

std::optional<RaySurfaceIntersection>
Scene::NaiveIntersect(const Ray &ray) const {
  Float t = INF;
  std::unique_ptr<RaySurfaceIntersection> get;
  for (auto &g : gs_) {
    auto ri = g.Intersect(ray);
    if (ri && (*ri).t < t) {
      t = (*ri).t;
      get = std::make_unique<RaySurfaceIntersection>(std::move(*ri));
    }
  }
  if (get) {
    RaySurfaceIntersection res(*get);
    return res;
  }
  return {};
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
