#ifndef _ZLI_CIE_HH_
#define _ZLI_CIE_HH_
#include "color.hh"
#include "diagram2d.hh"
#include "math.hh"

namespace zLi {
class CIE {
public:
  static const Float mXYZ2RGB[3][3];
  static const Float mRGB2XYZ[3][3];
  static constexpr const int NrXYZCMFSamples = 89;
  static const Float XYZ_CMF_Wavelength[NrXYZCMFSamples];
  static const Float XYZ_CMF[3][NrXYZCMFSamples];
  static RGBColor XYZ2RGB(const XYZColor &c);
  static XYZColor RGB2XYZ(const RGBColor &c);
};
}
#endif
