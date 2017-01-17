// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef _ZLI_INTEGRATOR_H_
#define _ZLI_INTEGRATOR_H_
#include <vector>

#include "light.h"
#include "scene.h"
#include "spectrum.h"

namespace zLi {

struct PathIntegrator {
  static Spectrum Li(const Scene &, const Ray &, int maxBounces = 8);
};

}  // namespace zLi
#endif
