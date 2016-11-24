// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef _ZLI_INTEGRATOR_HH_
#define _ZLI_INTEGRATOR_HH_
#include <vector>

#include "light.hh"
#include "scene.hh"
#include "spectrum.hh"

namespace zLi {

struct PathIntegrator {
  static Spectrum Li(const Scene &, const Ray &, int maxBounces = 8);
};

}  // namespace zLi
#endif
