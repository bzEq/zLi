#ifndef _ZLI_SHAPE_H_
#define _ZLI_SHAPE_H_
#include "geometry.h"
#include "ray.h"

#include <boost/optional.hpp>
#include <iostream>


namespace zLi {

struct Shape;

struct RayIntersection {
  const Float t;
  const Shape* shape;
};

struct Shape {
  virtual boost::optional<RayIntersection> Intersect(const Ray&) const = 0;
  virtual ~Shape() {}
};

} // end namespace zLi

#endif
