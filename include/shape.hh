#ifndef _ZLI_SHAPE_H_
#define _ZLI_SHAPE_H_
#include "geometry.hh"
#include "ray.hh"
#include "bsdf.hh"
#include "spectrum.hh"

#include <iostream>
#include <memory>


namespace zLi {

struct Shape;

struct RayIntersection {
  Float t;
  std::shared_ptr<Shape> shape;
  Ray ray;
  Ray SpawnRay(const Vector3f& d) const {
    return Ray(ray(t), d, EPSILON);
  }
};

struct Shape {
  virtual std::optional<RayIntersection> Intersect(const Ray&) = 0;
  virtual std::shared_ptr<BSDF> bsdf() = 0;
  virtual Spectrum Le() = 0;
  virtual Vector3f Normal(const Vector3f&) = 0;
  virtual ~Shape() {}
};

} // end namespace zLi

#endif
