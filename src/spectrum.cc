#include "spectrum.hh"
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
} // zLi
