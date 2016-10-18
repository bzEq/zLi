#ifndef _ZLI_SCENE_H_
#define _ZLI_SCENE_H_
#include "geometry.hh"
#include "spectrum.hh"
#include "shape.hh"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


namespace zLi {

class Scene {
public:
  static std::optional<Scene> SceneFromJson(const std::string file) {
    boost::property_tree::ptree json;
    boost::property_tree::read_json(file, json);
    return Scene();
  }
  Spectrum DirectLight(const Vector3f& position) const;
  Spectrum DirectLight(const RayIntersection& ri) const;
  std::optional<RayIntersection> Intersect(const Ray&) const;
};

} // end namespace zLi
#endif
