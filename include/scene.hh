#ifndef _ZLI_SCENE_HH_
#define _ZLI_SCENE_HH_
#include "geometry.hh"
#include "math.hh"
#include "spectrum.hh"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace zLi {

class Scene {
public:
  static std::optional<Scene> SceneFromJson(const std::string file) {
    boost::property_tree::ptree json;
    boost::property_tree::read_json(file, json);
    return Scene();
  }
  Spectrum DirectLight(const Vector3f &position) const;
  Spectrum DirectLight(const RaySurfaceIntersection &ri) const;
  std::optional<RaySurfaceIntersection> Intersect(const Ray &) const;

private:
  Scene() = default;
};

} // end namespace zLi
#endif
