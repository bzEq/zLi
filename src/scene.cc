#include "scene.hh"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <string>

namespace zLi {

Result<Scene> Scene::SceneFromJson(const std::string &file) {
  boost::property_tree::ptree json;
  boost::property_tree::read_json(file, json);
  return Ok(Scene());
}

} // zLi
