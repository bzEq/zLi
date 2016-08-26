#include "geometry.h"

#include <iostream>


int main()
{
  using namespace zLi;
  Vector3d v3;
  std::cout << v3.ToVector4() << std::endl;
  Vector4d v4(2., 3., 5., 0.5);
  std::cout << v4.ToVector3().value_or(Vector3d()) << std::endl;
  
  Matrix4x4d m(1, 0, 0, 1,
               0, 1, 0, 2,
               0, 0, 1, 3,
               0, 0, 0, 1);
  std::cout << m.Inverse().value_or(Matrix4x4d()) << std::endl;
  std::cout << m * v4 << std::endl;

  Quaternion4d q(1, 2, 2, 2);
  std::cout << q * q.Conjugation() << std::endl;
  std::cout << q * q.Inverse() << std::endl;
  return 0;
}
