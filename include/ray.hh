#ifndef _ZLI_RAY_H_
#define _ZLI_RAY_H_
#include "geometry.h"

#include <iostream>

namespace zLi {

struct Ray {
  Vector3f o, d;
  Float tmin, tmax;
  Ray() {}
  Ray(const Vector3f& o, const Vector3f& d, const Float tmin=0, const Float tmax=INF)
    : o(o), d(d), tmin(tmin), tmax(tmax) {}
  Vector3f operator()(const Float t) const { return o + t * d; }
};

} // end namespace zLi
#endif
