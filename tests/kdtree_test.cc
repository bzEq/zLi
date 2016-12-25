#include "kdtree.h"
#include "geometry.h"
#include "logging.h"
#include "sphere.h"
#include "triangle.h"

#include <iostream>

int main() {
  using namespace zLi;
  std::vector<Geometry> gs;
  gs.push_back(
      std::make_shared<Sphere>(Vector3f(-1, 0, 10), 0.5)->ImplGeometry());
  gs.push_back(
      std::make_shared<Sphere>(Vector3f(0, 0, 10), 0.5)->ImplGeometry());
  gs.push_back(
      std::make_shared<Sphere>(Vector3f(1, 0, 10), 0.5)->ImplGeometry());
  DEBUGLOG("building kdtree...");
  KdTree kt(KdTree::BuildKdTree(std::move(gs)));
  DEBUGLOG("build kdtree: ok");
  auto ray = Ray(Vector3f(0, 0, 0), Vector3f(0, 0, 1));
  auto res = kt.Intersect(ray);
  if (!res) {
    return 1;
  }
  std::cout << (*res).t << std::endl;
  return 0;
}
