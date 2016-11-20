#ifndef _ZLI_GEOMETRY_HH_
#define _ZLI_GEOMETRY_HH_
#include "boundbox.hh"
#include "bsdf.hh"
#include "math.hh"
#include "ray.hh"
#include "spectrum.hh"

#include <functional>
#include <iostream>
#include <memory>

namespace zLi {

struct Geometry;
struct RaySurfaceIntersection;

struct Geometry {
  // virtual std::optional<RaySurfaceIntersection> Intersect(const Ray&) = 0;
  // virtual std::shared_ptr<BSDF> bsdf() = 0;
  // virtual Spectrum Le() = 0;
  // virtual Vector3f Normal(const Vector3f&) = 0;
  // virtual ~Shape() {}
  std::function<std::optional<RaySurfaceIntersection>(const Ray &)> Intersect;
  std::function<Spectrum()> Le; // emitted radiance
  std::function<Spectrum()> R;  // reflectance
  // @args: position
  // @return: normal
  std::function<Vector3f(const Vector3f &)> Normal;
  std::function<BSDF()> bsdf;
  std::function<BoundBox()> Bounds;
};

struct RaySurfaceIntersection {
  Float t;
  Ray ray;
  Geometry g;
  Ray SpawnRay(const Vector3f &d) const { return Ray(ray(t), d, 0.001); }
  Ray SpawnRayTowards(const Vector3f &p) const {
    auto d = (p - ray(t)).Normalize();
    return SpawnRay(d);
  }
};

} // end namespace zLi

#endif
