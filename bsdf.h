// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef ZLI_BSDF_H_
#define ZLI_BSDF_H_
#include <functional>
#include <memory>
#include <tuple>

#include "core_math.h"

namespace zLi {

struct BRDF {
  enum class Type {
    Specular,
    Diffuse,
  };
  // @args: normal, wi
  // @return (pdf, wo)
  std::function<std::tuple<Float, Vector3f>(const Vector3f &, const Vector3f &)>
      pdf;

  // @args: normal, wi, wo
  // @return brdf
  std::function<Float(const Vector3f &, const Vector3f &, const Vector3f &)> f;
  std::function<BRDF::Type()> type;
};

struct BTDF {
  // @args: normal, wi
  // @return (pdf, wo)
  std::function<std::tuple<Float, Vector3f>(const Vector3f &, const Vector3f &)>
      pdf;

  // @args: normal, wi, wo
  // @return btdf
  std::function<Float(const Vector3f &, const Vector3f &, const Vector3f &)> f;
};

struct BSDF {
  BSDF() = default;
  BSDF(const BSDF &rhs) {
    if (rhs.brdf) {
      brdf = std::make_unique<BRDF>(*rhs.brdf);
    }
    if (rhs.btdf) {
      btdf = std::make_unique<BTDF>(*rhs.btdf);
    }
  }
  BSDF(BSDF &&) = default;
  std::unique_ptr<BRDF> brdf;
  std::unique_ptr<BTDF> btdf;
};

struct LambertianDiffuse {
  static std::tuple<Float, Vector3f> pdf(const Vector3f &normal,
                                         const Vector3f &wi);
  static Float f(const Vector3f &normal, const Vector3f &wi,
                 const Vector3f &wo);
  static BSDF ImplBSDF();
};

struct Specular {
  static std::tuple<Float, Vector3f> pdf(const Vector3f &normal,
                                         const Vector3f &wi);
  static Float f(const Vector3f &normal, const Vector3f &wi,
                 const Vector3f &wo);
  static BSDF ImplBSDF();
};

struct Refractive {
  static BSDF ImplBSDF(Float);
};

}  // namespace zLi
#endif
