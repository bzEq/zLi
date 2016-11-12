#include "kdtree.hh"
#include "geometry.hh"
#include "sphere.hh"
#include "triangle.hh"

#include <iostream>

int main() {
  using namespace zLi;
  std::vector<Geometry> gs;
  gs.push_back(std::make_shared<Sphere>(Vector3f(5, 5, 5), 2)->ImplGeometry());
  gs.push_back(std::make_shared<Sphere>(Vector3f(), 3)->ImplGeometry());
  KdTree kt(gs);
  auto res = kt.Intersect(Ray(Vector3f(0, 0, -10), Vector3f(0, 0, 1)));
  assert(res);
  std::cout << (*res).t << std::endl;
  return 0;
}
