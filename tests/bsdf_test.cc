#include "bsdf.hh"

#include <iostream>

using namespace zLi;

int main() {
  BSDF r = Refractive::ImplBSDF(1.2);
  Vector3f n(0, 1, 0);
  Vector3f wi(Vector3f(-1, 1, 0).Normalize());
  auto t = r.btdf->pdf(n, wi);
  std::cout << std::get<0>(t) << std::endl;
  std::cout << std::get<1>(t) << std::endl;
  return 0;
}
