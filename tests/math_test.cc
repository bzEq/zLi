#include "math.hh"
#include "CIE.hh"

#include <iostream>

int main() {
  using namespace zLi;
  Vector3f v3;
  std::cout << v3.ToVector4() << std::endl;
  Vector4f v4(2., 3., 5., 0.5);
  std::cout << *v4.ToVector3() << std::endl;

  Matrix4x4f m(1, 0, 0, 1, 0, 1, 0, 2, 0, 0, 1, 3, 0, 0, 0, 1);
  std::cout << *m.Inverse() << std::endl;
  std::cout << m * v4 << std::endl;

  Quaternion4f q(1, 2, 2, 2);
  std::cout << q * q.Conjugation() << std::endl;
  std::cout << q * q.Inverse() << std::endl;

  Vector3f v(1, 0, 0);
  Vector3f d(1, 1, 1);
  Line3f l(Vector3f(1, 1, 1), Vector3f(1, 1, 1));
  std::cout << "// Transform test" << std::endl;
  std::cout << PI << std::endl;
  std::cout << "// Translate test" << std::endl;
  std::cout << Translate(v, d) << std::endl;
  std::cout << *(TranslateTransform(d) * v) << std::endl;

  std::cout << "// Rotate test" << std::endl;
  std::cout << Rotate(v, d, 90) << std::endl;
  std::cout << *(RotateTransform(d, 90) * v) << std::endl;

  std::cout << Rotate(v, l, 90) << std::endl;
  std::cout << *(RotateTransform(l, 90) * v) << std::endl;

  std::cout << "CIE XYZ2RGB " << std::endl;
  Matrix4x4f cie(CIE::mXYZ2RGB);
  std::cout << cie << std::endl;
  std::cout << "CIE RGB2XYZ " << std::endl;
  std::cout << *cie.Inverse() << std::endl;

  Matrix4x4f mm(1, 2, 3, 0, 23, 4, 3, 0, 34, 343, 3, 0, 0, 0, 0, 1);
  Matrix4x4f mb(34, 23, 17, 0, 2, 3, 5, 0, 7, 4, 2, 0, 0, 0, 0, 1);
  Vector3f vv(1, 2, 3);
  std::cout << *(mm * vv) << std::endl;
  std::cout << *(mm.Inverse()) << std::endl;
  std::cout << mm * mb << std::endl;
  // std::cout << Rotate(v, l, 90) << std::endl;
  // std::cout << *(RotateTransform(l, 90) * v) << std::endl;
  return 0;
}
