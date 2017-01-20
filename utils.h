// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef ZLI_UTILS_H_
#define ZLI_UTILS_H_
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <string>
#include <vector>

#include "camera.h"
#include "core_math.h"
#include "light.h"
#include "spectrum.h"
#include "sphere.h"
#include "triangle.h"

namespace zLi {
class Utils {
public:
  static Vector3f Vector3FromJson(const boost::property_tree::ptree &json) {
    return Vector3f(json.get<Float>("x"), json.get<Float>("y"),
                    json.get<Float>("z"));
  }

  template <typename T>
  static std::vector<T> ListFromJson(const boost::property_tree::ptree &json) {
    std::vector<T> res;
    for (auto it = json.begin(); it != json.end(); ++it) {
      res.push_back(it->second.get<Float>(""));
    }
    return res;
  }

  static Spectrum SpectrumFromJson(const boost::property_tree::ptree &json) {
    std::vector<Float> xs(ListFromJson<Float>(json.get_child("xs")));
    std::vector<Float> ys(ListFromJson<Float>(json.get_child("ys")));
    assert(xs.size() == ys.size());
    return Spectrum(xs.data(), ys.data(), xs.size());
  }

  static BSDF BSDFFromJson(const boost::property_tree::ptree &json) {
    std::string type = json.get<std::string>("type");
    if (type == "specular") {
      return Specular::ImplBSDF();
    } else if (type == "refractive") {
      return Refractive::ImplBSDF(json.get<Float>("index"));
    }
    return LambertianDiffuse::ImplBSDF();
  }
};
}  // namespace zLi
#endif
