#include "bsdf.hh"

namespace zLi {

std::tuple<Float, Vector3f> LambertianDiffuse::pdf(const Vector3f &normal,
                                                   const Vector3f &wi) {
  if (normal * wi >= 0)
    return std::make_tuple(0, Vector3f());
  Vector3f wo = SampleFromHemiSphere();
  if (wo * normal <= 0)
    wo = -wo;
  return std::make_tuple(std::abs(wo * normal) / PI, wo);
}

Float LambertianDiffuse::f(const Vector3f &normal, const Vector3f &wi,
                           const Vector3f &wo) {
  if (normal * wo <= 0 || normal * wi >= 0)
    return 0;
  return 1 / PI;
}

BSDF::Type LambertianDiffuse::type() { return BSDF::Type::Diffuse; }

BSDF LambertianDiffuse::ImplBSDF() {
  return BSDF{
      .pdf = std::bind(&LambertianDiffuse::pdf, std::placeholders::_1,
                       std::placeholders::_2),

      .f = std::bind(&LambertianDiffuse::f, std::placeholders::_1,
                     std::placeholders::_2, std::placeholders::_3),
      .type = std::bind(&LambertianDiffuse::type),
  };
}

std::tuple<Float, Vector3f> Specular::pdf(const Vector3f &normal,
                                          const Vector3f &wi) {
  if (normal * wi >= 0)
    return std::make_tuple(0, Vector3f());
  auto wo = (wi - normal * (2 * (wi * normal))); // if noraml and wi r unit
  // vectors, wo should be an unit
  // vector too
  return std::make_tuple(1.f, wo);
}

Float Specular::f(const Vector3f &noraml, const Vector3f &wi,
                  const Vector3f &wo) {
  return 0;
}
}
