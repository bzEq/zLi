#include "camera.hh"
#include "math.hh"

#include <iostream>

int main() {
  using namespace zLi;
  zLi::PerspectiveCamera pc(Vector3f(0, 0, -10), Vector3f(0, 0, 1),
                            Vector3f(0, 1, 0));
  std::cout << "base vector" << std::endl;
  std::cout << pc.right << std::endl;
  std::cout << pc.up << std::endl;
  std::cout << pc.front << std::endl;
  std::cout << "cam2wd" << std::endl;
  std::cout << *(pc.cam2wd * Vector3f()) << std::endl;
  std::cout << "wd2cam" << std::endl;
  std::cout << *(pc.wd2cam * pc.e) << std::endl;
  std::cout << "ray" << std::endl;
  Ray ray = pc.GenerateRay(0.5, 0.5);
  std::cout << ray.o << " " << ray.d << std::endl;
  return 0;
}
