#include "triangle.h"

#include <memory>

int main() {
  using namespace zLi;
  Vector3f v(-1, -1, 0);
  auto tri = std::make_shared<Triangle>(
      Vector3f(0, 0, 2) + v, Vector3f(3, 0, 2) + v, Vector3f(0, 4, 2) + v);
  Ray r(Vector3f(0, 0, 0), Vector3f(0, 0, 1));
  auto test = tri->Intersect(r);
  if (test)
    std::cout << (*test).t << std::endl;
  return 0;
}
