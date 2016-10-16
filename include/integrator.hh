#ifndef _ZLI_INTEGRATOR_H_
#define _ZLI_INTEGRATOR_H_
#include "spectrum.h"
#include "scene.h"
#include "light.h"

#include <vector>

namespace zLi {

struct PathIntegrator {
  static Spectrum Li(const Scene&,
                     const Ray&, const int);
};

} // end namespace zLi
#endif
