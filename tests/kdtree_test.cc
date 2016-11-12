#include "kdtree.hh"
#include "geometry.hh"
#include "sphere.hh"
#include "triangle.hh"

#include <iostream>

int main() {
  using namespace zLi;
  std::vector<Geometry> gs;
  gs.push_back(
      std::make_shared<Sphere>(Vector3f(10, 10, 100), 2)->ImplGeometry());
  gs.push_back(std::make_shared<Sphere>(Vector3f(5, 5, 5), 2)->ImplGeometry());
  gs.push_back(
      std::make_shared<Sphere>(Vector3f(52, 520, 53), 2)->ImplGeometry());
  gs.push_back(std::make_shared<Sphere>(Vector3f(), 3)->ImplGeometry());
  gs.push_back(
      std::make_shared<Sphere>(Vector3f(1800, 5200, 5300), 2)->ImplGeometry());

  gs.push_back(std::make_shared<Triangle>(Vector3f(134, 234, 23),
                                          Vector3f(0, 0, 0),
                                          Vector3f(100, 234, 78))
                   ->ImplGeometry());
  gs.push_back(std::make_shared<Triangle>(Vector3f(1340, 2340, 230),
                                          Vector3f(100, 8374, 234),
                                          Vector3f(100, 234, 78))
                   ->ImplGeometry());
  gs.push_back(std::make_shared<Triangle>(Vector3f(-1, -1, -6),
                                          Vector3f(0, 2, -8),
                                          Vector3f(1, -1, -8))
                   ->ImplGeometry());
  KdTree kt(gs);
  auto res = kt.Intersect(Ray(Vector3f(0, 0, -10), Vector3f(0, 0, 1)));
  assert(res);
  std::cout << (*res).t << std::endl;
  return 0;
}
