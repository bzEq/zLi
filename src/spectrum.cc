#include "spectrum.hh"
#include "CIE.hh"
#include "color.hh"
#include "diagram2d.hh"

namespace zLi {

const int Spectrum::NrSpectralSamples = 11;
const Float Spectrum::DefaultSampleX[Spectrum::NrSpectralSamples] = {
    380, 420, 460, 500, 540, 580, 620, 660, 700, 740, 780};
const Float Spectrum::DefaultSampleY[Spectrum::NrSpectralSamples] = {0};

Spectrum::Spectrum()
    : spd_(Spectrum::DefaultSampleX, Spectrum::DefaultSampleY,
           Spectrum::NrSpectralSamples) {}

Spectrum::Spectrum(Float v)
    : spd_(Spectrum::DefaultSampleX, v, Spectrum::NrSpectralSamples) {}

Spectrum::Spectrum(const Float *xs, const Float *ys, size_t l)
    : spd_(xs, ys, l) {}

// Riemann sum
XYZColor Spectrum::ToXYZ() const {
  Float x(0), y(0), z(0);
  Float scale = (DefaultSampleX[NrSpectralSamples - 1] - DefaultSampleX[0]) /
                (CIE::XYZ_CMF_Y_INT * NrSpectralSamples);
  for (int i = 0; i < CIE::NrXYZCMFSamples; ++i) {
    Float l = spd_.Query(CIE::XYZ_CMF_Wavelength[i]);
    x += l * CIE::XYZ_CMF[0][i];
    y += l * CIE::XYZ_CMF[1][i];
    z += l * CIE::XYZ_CMF[2][i];
  }
  return XYZColor(x * scale, y * scale, z * scale);
}

RGBColor Spectrum::ToRGB() const { return CIE::XYZ2RGB(ToXYZ()); }
xyYColor Spectrum::ToxyY() const { return CIE::XYZ2xyY(ToXYZ()); }
} // zLi
