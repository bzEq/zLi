// Copyright (c) 2016 Kai Luo. All rights reserved.

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <string>

#include "geometry.h"
#include "scene.h"
#include "sphere.h"
#include "triangle.h"
#include "utils.h"

namespace zLi {

std::vector<Geometry>
Scene::GeometriesFromJson(const boost::property_tree::ptree &json) {
  std::vector<Geometry> gs;
  for (auto it = json.begin(); it != json.end(); ++it) {
    auto s = it->second;
    auto type = s.get<std::string>("type");
    KL_DEBUG("geometry type: %s", type.c_str());
    if (type == "sphere") {
      auto res = SphereFromJson(s);
      if (res) {
        gs.push_back(std::make_shared<Sphere>(std::move(*res))->ImplGeometry());
      } else {
        KL_WARN(res.Err().ToCString());
      }
    } else if (type == "triangle") {
      auto res = TriangleFromJson(s);
      if (res) {
        gs.push_back(
            std::make_shared<Triangle>(std::move(*res))->ImplGeometry());
      } else {
        KL_WARN(res.Err().ToCString());
      }
    }
  }
  return gs;
}

std::vector<Light>
Scene::LightsFromJson(const boost::property_tree::ptree &json) {
  std::vector<Light> ls;
  for (auto it = json.begin(); it != json.end(); ++it) {
    auto s = it->second;
    auto type = s.get<std::string>("type");
    KL_DEBUG("light type: %s", type.c_str());
    if (type == "point") {
      auto res = PointLightFromJson(s);
      if (res) {
        ls.push_back(
            std::make_shared<PointLight>(std::move(*res))->ImplLight());
      } else {
        KL_WARN(res.Err().ToCString());
      }
    }
  }
  return ls;
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

kl::Result<Triangle>
Scene::TriangleFromJson(const boost::property_tree::ptree &json) {
  try {
    Vector3f a = Utils::Vector3FromJson(json.get_child("a"));
    Vector3f b = Utils::Vector3FromJson(json.get_child("b"));
    Vector3f c = Utils::Vector3FromJson(json.get_child("c"));
    Spectrum Le(Utils::SpectrumFromJson(json.get_child("le")));
    Spectrum R(Utils::SpectrumFromJson(json.get_child("R")));
    return kl::Ok(
        Triangle(a, b, c, Le, R, Utils::BSDFFromJson(json.get_child("bsdf"))));
  } catch (const std::exception &e) {
    return kl::Err(e.what());
  }
}

kl::Result<Scene> Scene::SceneFromJson(const std::string &file) {
  boost::property_tree::ptree json;
  try {
    boost::property_tree::read_json(file, json);
    // read geometries
    std::vector<Geometry> gs(GeometriesFromJson(json.get_child("geometry")));
    std::vector<Light> ls(LightsFromJson(json.get_child("light")));
    // Scene scene((KdTree(gs)));
    auto rep(gs);
    Scene scene;
    scene.gs_ = std::move(gs);
    scene.lights_ = std::move(ls);
    scene.kdt_ = std::make_unique<KdTree>(KdTree::BuildKdTree(std::move(rep)));
    // add lights
    // add camera
    scene.camera_ = CameraFromJson(json.get_child("camera"));
    return kl::Ok(std::move(scene));
  } catch (std::exception &e) {
    return kl::Err(e.what());
  }
}

Ray Scene::GenerateRay(Float u, Float v) { return camera_.GenerateRay(u, v); }

kl::Result<Sphere>
Scene::SphereFromJson(const boost::property_tree::ptree &json) {
  try {
    Float radius = json.get<Float>("radius");
    Vector3f center(Utils::Vector3FromJson(json.get_child("center")));
    Spectrum Le(Utils::SpectrumFromJson(json.get_child("le")));
    Spectrum R(Utils::SpectrumFromJson(json.get_child("R")));
    return kl::Ok(Sphere(center, radius, Le, R,
                         Utils::BSDFFromJson(json.get_child("bsdf"))));
  } catch (const std::exception &e) {
    return kl::Err(e.what());
  }
}

kl::Result<PointLight>
Scene::PointLightFromJson(const boost::property_tree::ptree &json) {
  try {
    Vector3f p(Utils::Vector3FromJson(json.get_child("position")));
    Spectrum le(Utils::SpectrumFromJson(json.get_child("le")));
    return kl::Ok(PointLight(p, le));
  } catch (const std::exception &e) {
    return kl::Err(e.what());
  }
}

Spectrum Scene::DirectLight(const RaySurfaceIntersection &ri) const {
  Spectrum le(ri.g.Le());
  for (auto &l : lights_) {
    le += l.Le(ri);
  }
  return le;
}

std::optional<RaySurfaceIntersection> Scene::Intersect(const Ray &ray) const {
  if (kdt_) {
    return kdt_->Intersect(ray);
  }
  return NaiveIntersect(ray);
  // auto r1 = NaiveIntersect(ray);
  // if (r0) {
  //   assert(r1);
  //   assert((*r0).t == (*r1).t);
  // }
  // if (r1) {
  //   if (!r0) {
  //     WARN("r1 found, but r0, t = %f. ray: ", (*r1).t);
  //     std::cerr << ray.o << " " << ray.d;
  //     WARN(" %f %f", ray.tmin, ray.tmax);
  //     auto world(kdt_->World());
  //     std::cerr << world.min_point << " " << world.max_point << std::endl;
  //     assert(world.Intersect(ray));
  //     assert(false);
  //   }
  // }
}

std::optional<RaySurfaceIntersection>
Scene::NaiveIntersect(const Ray &ray) const {
  std::unique_ptr<RaySurfaceIntersection> get;
  for (auto &g : gs_) {
    auto ri = g.Intersect(ray);
    if (ri && (!get || (*ri).t < get->t)) {
      get = std::make_unique<RaySurfaceIntersection>(std::move(*ri));
    }
  }
  if (get) {
    return *get;
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

}  // namespace zLi
