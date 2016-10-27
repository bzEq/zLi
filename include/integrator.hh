#ifndef _ZLI_INTEGRATOR_HH_
#define _ZLI_INTEGRATOR_HH_
#include "spectrum.hh"
#include "scene.hh"
#include "light.hh"

#include <vector>

namespace zLi {

struct PathIntegrator {
  static Spectrum Li(const Scene&,
                     const Ray&, int);
};

} // end namespace zLi
#endif
