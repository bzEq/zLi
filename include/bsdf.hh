#ifndef _ZLI_BSDF_HH_
#define _ZLI_BSDF_HH_
#include "math.hh"

#include <functional>
#include <memory>

namespace zLi {

struct BSDF {
  enum class Type {
    Specular,
    Refractive,
    Diffuse,
  };
  // type of bsdf
  std::function<Type()> type;

  // @args: normal, wi
  // @return (pdf, wo)
  std::function<std::tuple<Float, Vector3f>(const Vector3f &, const Vector3f &)>
      pdf;

  // @args: normal, wi, wo
  // @return bsdf
  std::function<Float(const Vector3f &, const Vector3f &, const Vector3f &)> f;
};

struct LambertianDiffuse {
  static std::tuple<Float, Vector3f> pdf(const Vector3f &normal,
                                         const Vector3f &wi);
  static Float f(const Vector3f &normal, const Vector3f &wi,
                 const Vector3f &wo);
  static BSDF::Type type();
  static BSDF ImplBSDF();
};

struct Specular {
  static std::tuple<Float, Vector3f> pdf(const Vector3f &normal,
                                         const Vector3f &wi);
  static Float f(const Vector3f &normal, const Vector3f &wi,
                 const Vector3f &wo);
  static BSDF::Type type();
  static BSDF ImplBSDF();
};

struct Refractive {
  static std::tuple<Float, Vector3f> pdf(const Vector3f &normal,
                                         const Vector3f &wi);
  static BSDF ImplBSDF(Float);
};

} // end namespace zLi

#endif
