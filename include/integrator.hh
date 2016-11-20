#ifndef _ZLI_INTEGRATOR_HH_
#define _ZLI_INTEGRATOR_HH_
#include "light.hh"
#include "scene.hh"
#include "spectrum.hh"

#include <vector>

namespace zLi {

struct PathIntegrator {
  static Spectrum Li(const Scene &, const Ray &, int maxBounces = 8);
};

} // end namespace zLi
#endif
