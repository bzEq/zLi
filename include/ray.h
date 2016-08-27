#ifndef _ZLI_RAY_H_
#define _ZLI_RAY_H_
#include "geometry.h"

#include <iostream>

namespace zLi {

struct Ray {
  Vector3d o, d;
  Float tmin, tmax;
  Ray() {}
  Ray(const Vector3d& o, const Vector3d& d): o(o), d(d), tmin(0), tmax(INF) {}
  Ray(const Vector3d& o, const Vector3d& d, const Float tmin, const Float tmax): o(o), d(d), tmin(tmin), tmax(tmax) {}  
  Vector3f operator()(const Float t) const { return o + t * d; }
};

} // end namespace zLi
#endif
