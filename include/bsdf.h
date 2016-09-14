#ifndef _ZLI_BSDF_H_
#define _ZLI_BSDF_H_
#include "geometry.h"
#include "sample.h"


namespace zLi {

struct BSDF {
  virtual std::tuple< Float, std::optional<Vector3f> > 
    SampleDirection(const Vector3f& normal, const Vector3f& wo) const = 0; // return (pdf, wi) tuple
  virtual Float f(const Vector3f& normal, const Vector3f& wi, const Vector3f& wo) const = 0;
  virtual ~BSDF() {}
};

struct LambertianDiffuse: public BSDF {
  std::tuple< Float, std::optional<Vector3f> > SampleDirection(const Vector3f& normal, const Vector3f& wo) const {
    if (normal * wo <= 0) return std::make_tuple(0, std::optional<Vector3f>());
    Vector3f wi = SampleFromHemiSphere();
    if (wi * normal >= 0) wi = -wi;
    return std::make_tuple(std::abs(wi*normal)/PI, wi);
  }
  Float f(const Vector3f& normal, const Vector3f& wi, const Vector3f& wo) const {
    if (normal * wo <= 0 || normal * wi >= 0) return 0;
    return 1/PI;
  }
  ~LambertianDiffuse() {}
};

} // end namespace zLi
  
#endif
