#include "CIE.hh"
#include "spectrum.hh"

#include <iostream>

int main() {
  using namespace zLi;
  std::cout << CIE::NrXYZCMFSamples << std::endl;
  std::cout << CIE::RGB2XYZ(RGBColor(0, 0, 1)) << std::endl;
  std::cout << CIE::XYZ2RGB(XYZColor(0, 0, 1)) << std::endl;
  auto s = Spectrum(0.01);
  std::cout << s.ToXYZ() << std::endl;
  std::cout << s.ToRGB() << std::endl;
  std::cout << CIE::RGB2XYZ(s.ToRGB()) << std::endl;
  return 0;
}
