#include "kdtree.hh"
#include "geometry.hh"
#include "logging.hh"
#include "sphere.hh"
#include "triangle.hh"

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
  DEBUG("building kdtree...");
  KdTree kt(KdTree::BuildKdTree(std::move(gs)));
  DEBUG("build kdtree: ok");
  auto ray = Ray(Vector3f(0, 0, 0), Vector3f(0, 0, 1));
  auto res = kt.Intersect(ray);
  if (!res) {
    return 1;
  }
  std::cout << (*res).t << std::endl;
  return 0;
}
