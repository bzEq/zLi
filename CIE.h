// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef ZLI_CIE_H_
#define ZLI_CIE_H_
#include "color.h"
#include "core_math.h"
#include "diagram2d.h"

namespace zLi {
class CIE {
public:
  static const Float mXYZ2RGB[3][3];
  static const Float mRGB2XYZ[3][3];
  static constexpr const int NrXYZCMFSamples = 89;
  static const Float XYZ_CMF_Wavelength[NrXYZCMFSamples];
  static const Float XYZ_CMF[3][NrXYZCMFSamples];
  static const Float XYZ_CMF_Y_INT;
  static RGBColor XYZ2RGB(const XYZColor &);
  static xyYColor XYZ2xyY(const XYZColor &);
  static XYZColor RGB2XYZ(const RGBColor &);
  static RGBColor xyY2RGB(const xyYColor &);
};
}  // namespace zLi
#endif
