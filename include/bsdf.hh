#ifndef _ZLI_BSDF_HH_
#define _ZLI_BSDF_HH_
#include "math.hh"

#include <functional>
#include <memory>

namespace zLi {

struct BSDF {
  // @args: normal, wi
  // @return (pdf, wo)
  std::function<std::tuple<Float, Vector3f>(const Vector3f &, const Vector3f &)>
      pdf;
  // @args: normal, wi, wo
  // @return bsdf
  std::function<Float(const Vector3f &, const Vector3f &, const Vector3f &)> f;
};

struct LambertianDiffuse
    : public std::enable_shared_from_this<LambertianDiffuse> {
  std::tuple<Float, Vector3f> pdf(const Vector3f &normal, const Vector3f &wi) {
    if (normal * wi >= 0)
      return std::make_tuple(0, Vector3f());
    Vector3f wo = SampleFromHemiSphere();
    if (wo * normal <= 0)
      wo = -wo;
    return std::make_tuple(std::abs(wo * normal) / PI, wo);
  }
  Float f(const Vector3f &normal, const Vector3f &wi, const Vector3f &wo) {
    if (normal * wo <= 0 || normal * wi >= 0)
      return 0;
    return 1 / PI;
  }
  BSDF ImplBSDF() {
    return BSDF{
        .pdf = std::bind(&LambertianDiffuse::pdf, shared_from_this(),
                         std::placeholders::_1, std::placeholders::_2),

        .f = std::bind(&LambertianDiffuse::f, shared_from_this(),
                       std::placeholders::_1, std::placeholders::_2,
                       std::placeholders::_3),
    };
  }
};

} // end namespace zLi

#endif
