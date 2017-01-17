// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef _ZLI_RAY_H_
#define _ZLI_RAY_H_
#include <iostream>

#include "core_math.h"

namespace zLi {

struct Ray {
  Vector3f o, d;
  Float tmin, tmax;
  Ray() = default;
  Ray(const Vector3f &o, const Vector3f &d, Float tmin = 0, Float tmax = INF)
      : o(o), d(d), tmin(tmin), tmax(tmax) {}
  Vector3f operator()(Float t) const { return o + t * d; }
};

}  // namespace zLi
#endif
