#include "geometry.h"

#include <iostream>


int main()
{
  using namespace zLi;
  Vector3f v3;
  std::cout << v3.ToVector4() << std::endl;
  Vector4f v4(2., 3., 5., 0.5);
  std::cout << v4.ToVector3().value_or(Vector3f()) << std::endl;
  
  Matrix4x4f m(1, 0, 0, 1,
               0, 1, 0, 2,
               0, 0, 1, 3,
               0, 0, 0, 1);
  std::cout << m.Inverse().value_or(Matrix4x4f()) << std::endl;
  std::cout << m * v4 << std::endl;

  Quaternion4f q(1, 2, 2, 2);
  std::cout << q * q.Conjugation() << std::endl;
  std::cout << q * q.Inverse() << std::endl;

  Vector3f v(1, 0, 0);
  Vector3f d(0, 0, 1);
  std::cout << Rotate(d, 90, v) << std::endl;
  std::cout << *(Rotate(d, 90) * v) << std::endl;
  return 0;
}
