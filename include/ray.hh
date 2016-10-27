#ifndef _ZLI_RAY_HH_
#define _ZLI_RAY_HH_
#include "math.hh"

#include <iostream>

namespace zLi {

struct Ray {
  Vector3f o, d;
  Float tmin, tmax;
  Ray() = default;
  Ray(const Vector3f& o, const Vector3f& d, Float tmin=0, Float tmax=INF)
    : o(o), d(d), tmin(tmin), tmax(tmax) { }
  Vector3f operator()(Float t) const { return o + t * d; }
};

} // end namespace zLi
#endif
