// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef _ZLI_GEOMETRY_HH_
#define _ZLI_GEOMETRY_HH_
#include <functional>
#include <iostream>
#include <memory>

#include "boundbox.hh"
#include "bsdf.hh"
#include "math.hh"
#include "ray.hh"
#include "spectrum.hh"

namespace zLi {

struct Geometry;
struct RaySurfaceIntersection;

struct Geometry {
  std::function<std::optional<RaySurfaceIntersection>(const Ray &)> Intersect;

  // emitted radiance
  std::function<Spectrum()> Le;

  // reflectance
  std::function<Spectrum()> R;

  // @args: position
  // @return: normal
  std::function<Vector3f(const Vector3f &)> Normal;

  std::function<BSDF()> bsdf;

  std::function<BoundBox()> Bounds;
};

inline void CheckGeometry(const Geometry &g) {
  assert(g.Intersect);
  assert(g.Le);
  assert(g.R);
  assert(g.Normal);
  assert(g.bsdf);
  assert(g.Bounds);
}

struct RaySurfaceIntersection {
  Float t;
  Ray ray;
  Geometry g;
  Ray SpawnRay(const Vector3f &d) const { return Ray(ray(t), d, 1e-3); }
  Ray SpawnRayTowards(const Vector3f &p) const {
    auto d = (p - ray(t)).Normalize();
    return SpawnRay(d);
  }
};

}  // end namespace zLi

#endif
