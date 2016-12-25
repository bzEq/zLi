// Copyright (c) 2016 Kai Luo. All rights reserved.

#include "CIE.h"
#include "core_math.h"

namespace zLi {
// https://en.wikipedia.org/wiki/CIE_1931_color_space
const Float CIE::mXYZ2RGB[3][3] = {
    {3.240479, -1.537150, -0.498535},
    {-0.969256, 1.875991, 0.041556},
    {0.055648, -0.204043, 1.057311},
};
const Float CIE::mRGB2XYZ[3][3] = {
    {0.412453, 0.35758, 0.180423},
    {0.212671, 0.71516, 0.0721688},
    {0.0193338, 0.119194, 0.950227},
};

// const int CIE::NrXYZCMFSamples = 89;

const Float CIE::XYZ_CMF_Wavelength[CIE::NrXYZCMFSamples] = {
    390, 395, 400, 405, 410, 415, 420, 425, 430, 435, 440, 445, 450, 455, 460,
    465, 470, 475, 480, 485, 490, 495, 500, 505, 510, 515, 520, 525, 530, 535,
    540, 545, 550, 555, 560, 565, 570, 575, 580, 585, 590, 595, 600, 605, 610,
    615, 620, 625, 630, 635, 640, 645, 650, 655, 660, 665, 670, 675, 680, 685,
    690, 695, 700, 705, 710, 715, 720, 725, 730, 735, 740, 745, 750, 755, 760,
    765, 770, 775, 780, 785, 790, 795, 800, 805, 810, 815, 820, 825, 830};

const Float CIE::XYZ_CMF[3][CIE::NrXYZCMFSamples] = {
    {3.769647E-03, 9.382967E-03, 2.214302E-02, 4.742986E-02, 8.953803E-02,
     1.446214E-01, 2.035729E-01, 2.488523E-01, 2.918246E-01, 3.227087E-01,
     3.482554E-01, 3.418483E-01, 3.224637E-01, 2.826646E-01, 2.485254E-01,
     2.219781E-01, 1.806905E-01, 1.291920E-01, 8.182895E-02, 4.600865E-02,
     2.083981E-02, 7.097731E-03, 2.461588E-03, 3.649178E-03, 1.556989E-02,
     4.315171E-02, 7.962917E-02, 1.268468E-01, 1.818026E-01, 2.405015E-01,
     3.098117E-01, 3.804244E-01, 4.494206E-01, 5.280233E-01, 6.133784E-01,
     7.016774E-01, 7.967750E-01, 8.853376E-01, 9.638388E-01, 1.051011E+00,
     1.109767E+00, 1.143620E+00, 1.151033E+00, 1.134757E+00, 1.083928E+00,
     1.007344E+00, 9.142877E-01, 8.135565E-01, 6.924717E-01, 5.755410E-01,
     4.731224E-01, 3.844986E-01, 2.997374E-01, 2.277792E-01, 1.707914E-01,
     1.263808E-01, 9.224597E-02, 6.639960E-02, 4.710606E-02, 3.292138E-02,
     2.262306E-02, 1.575417E-02, 1.096778E-02, 7.608750E-03, 5.214608E-03,
     3.569452E-03, 2.464821E-03, 1.703876E-03, 1.186238E-03, 8.269535E-04,
     5.758303E-04, 4.058303E-04, 2.856577E-04, 2.021853E-04, 1.438270E-04,
     1.024685E-04, 7.347551E-05, 5.259870E-05, 3.806114E-05, 2.758222E-05,
     2.004122E-05, 1.458792E-05, 1.068141E-05, 7.857521E-06, 5.768284E-06,
     4.259166E-06, 3.167765E-06, 2.358723E-06, 1.762465E-06},
    {4.146161E-04, 1.059646E-03, 2.452194E-03, 4.971717E-03, 9.079860E-03,
     1.429377E-02, 2.027369E-02, 2.612106E-02, 3.319038E-02, 4.157940E-02,
     5.033657E-02, 5.743393E-02, 6.472352E-02, 7.238339E-02, 8.514816E-02,
     1.060145E-01, 1.298957E-01, 1.535066E-01, 1.788048E-01, 2.064828E-01,
     2.379160E-01, 2.850680E-01, 3.483536E-01, 4.277595E-01, 5.204972E-01,
     6.206256E-01, 7.180890E-01, 7.946448E-01, 8.575799E-01, 9.071347E-01,
     9.544675E-01, 9.814106E-01, 9.890228E-01, 9.994608E-01, 9.967737E-01,
     9.902549E-01, 9.732611E-01, 9.424569E-01, 8.963613E-01, 8.587203E-01,
     8.115868E-01, 7.544785E-01, 6.918553E-01, 6.270066E-01, 5.583746E-01,
     4.895950E-01, 4.229897E-01, 3.609245E-01, 2.980865E-01, 2.416902E-01,
     1.943124E-01, 1.547397E-01, 1.193120E-01, 8.979594E-02, 6.671045E-02,
     4.899699E-02, 3.559982E-02, 2.554223E-02, 1.807939E-02, 1.261573E-02,
     8.661284E-03, 6.027677E-03, 4.195941E-03, 2.910864E-03, 1.995557E-03,
     1.367022E-03, 9.447269E-04, 6.537050E-04, 4.555970E-04, 3.179738E-04,
     2.217445E-04, 1.565566E-04, 1.103928E-04, 7.827442E-05, 5.578862E-05,
     3.981884E-05, 2.860175E-05, 2.051259E-05, 1.487243E-05, 1.080001E-05,
     7.863920E-06, 5.736935E-06, 4.211597E-06, 3.106561E-06, 2.286786E-06,
     1.693147E-06, 1.262556E-06, 9.422514E-07, 7.053860E-07},
    {1.847260E-02, 4.609784E-02, 1.096090E-01, 2.369246E-01, 4.508369E-01,
     7.378822E-01, 1.051821E+00, 1.305008E+00, 1.552826E+00, 1.748280E+00,
     1.917479E+00, 1.918437E+00, 1.848545E+00, 1.664439E+00, 1.522157E+00,
     1.428440E+00, 1.250610E+00, 9.991789E-01, 7.552379E-01, 5.617313E-01,
     4.099313E-01, 3.105939E-01, 2.376753E-01, 1.720018E-01, 1.176796E-01,
     8.283548E-02, 5.650407E-02, 3.751912E-02, 2.438164E-02, 1.566174E-02,
     9.846470E-03, 6.131421E-03, 3.790291E-03, 2.327186E-03, 1.432128E-03,
     8.822531E-04, 5.452416E-04, 3.386739E-04, 2.117772E-04, 1.335031E-04,
     8.494468E-05, 5.460706E-05, 3.549661E-05, 2.334738E-05, 1.554631E-05,
     1.048387E-05, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00,
     0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00,
     0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00,
     0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00,
     0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00,
     0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00,
     0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00,
     0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00,
     0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00},
};

const Float CIE::XYZ_CMF_Y_INT = 111.778458349f;

RGBColor CIE::XYZ2RGB(const XYZColor &xyz) {
  RGBColor rgb;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      rgb[i] += mXYZ2RGB[i][j] * xyz[j];
    }
  }
  return rgb;
}

XYZColor CIE::RGB2XYZ(const RGBColor &rgb) {
  XYZColor xyz;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      xyz[i] += mRGB2XYZ[i][j] * rgb[j];
    }
  }
  return xyz;
}

xyYColor CIE::XYZ2xyY(const XYZColor &xyz) {
  Float X = xyz.x, Y = xyz.y, Z = xyz.z;
  Float s = X + Y + Z;
  if (s == 0) {
    return xyYColor();
  }
  return xyYColor(X / s, Y / s, Y);
}

RGBColor CIE::xyY2RGB(const xyYColor &xyY) {
  Float x = xyY.x, y = xyY.y, Y = xyY.Y;
  return XYZ2RGB(XYZColor(Y * x / y, Y, Y * (1 - x - y) / y));
}
}  // namespace zLi
