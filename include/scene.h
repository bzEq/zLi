#ifndef _ZLI_SCENE_H_
#define _ZLI_SCENE_H_
#include "spectrum.h"
#include "shape.h"

#include <boost/optional.hpp>


namespace zLi {

struct Scene {
  Spectrum DirectLight(const Vector3f& position) const;
  Spectrum DirectLight(const RayIntersection& ri) const;
  boost::optional<RayIntersection> Intersect(const Ray&) const;  
};

} // end namespace zLi
#endif
