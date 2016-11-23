#include "boundbox.hh"

int main() {
  using namespace zLi;
  BoundBox b(Vector3f(-900000, -1, -900000), Vector3f(900000, 1.9e6, 900000));
  Ray ray(Vector3f(-0.514135, 0.0694799, 10.0954),
          Vector3f(0.618431, 0.413909, 0.667999), 0.001000, INF);
  std::cout << b.InBox(ray(ray.tmin)) << std::endl;
  auto ri = b.Intersect(ray);
  assert(ri);
  std::cout << std::get<0>(*ri) << std::endl;
  std::cout << std::get<1>(*ri) << std::endl;
  return 0;
}
