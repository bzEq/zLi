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

XYZColor Spectrum::ToXYZ() const {
  Float x(0), y(0), z(0);
  for (int i = 0; i < CIE::NrXYZCMFSamples; ++i) {
    Float l = spd_.Query(CIE::XYZ_CMF_Wavelength[i]);
    x += l * CIE::XYZ_CMF[0][i];
    y += l * CIE::XYZ_CMF[0][i];
    z += l * CIE::XYZ_CMF[0][i];
  }
  return XYZColor(x, y, z);
}

RGBColor Spectrum::ToRGB() const { return CIE::XYZ2RGB(ToXYZ()); }
} // zLi
