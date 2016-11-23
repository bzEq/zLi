#include "bsdf.hh"

#include <memory>

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
  if (normal * wo <= 0 || normal * wi >= 0) {
    return 0;
  }
  return 1 / PI;
}

BSDF LambertianDiffuse::ImplBSDF() {
  BSDF ret;
  ret.brdf = std::make_unique<BRDF>(BRDF{
      .pdf = std::bind(&LambertianDiffuse::pdf, std::placeholders::_1,
                       std::placeholders::_2),

      .f = std::bind(&LambertianDiffuse::f, std::placeholders::_1,
                     std::placeholders::_2, std::placeholders::_3),
      .type = []() { return BRDF::Type::Diffuse; },
  });
  return ret;
}

BSDF Specular::ImplBSDF() {
  BSDF ret;
  ret.brdf = std::make_unique<BRDF>(BRDF{
      .pdf = std::bind(&Specular::pdf, std::placeholders::_1,
                       std::placeholders::_2),

      .f = std::bind(&Specular::f, std::placeholders::_1, std::placeholders::_2,
                     std::placeholders::_3),
      .type = []() { return BRDF::Type::Specular; },
  });
  return ret;
}

std::tuple<Float, Vector3f> Specular::pdf(const Vector3f &normal,
                                          const Vector3f &wi) {
  if (normal * wi >= 0) {
    return std::make_tuple(0, Vector3f());
  }
  auto wo = (wi - normal * (2 * (wi * normal))); // if noraml and wi r unit
  // vectors, wo should be an unit
  // vector too
  return std::make_tuple(1.f, wo);
}

Float Specular::f(const Vector3f &noraml, const Vector3f &wi,
                  const Vector3f &wo) {
  return 1;
}

BSDF Refractive::ImplBSDF(Float index) {
  BSDF ret;
  ret.brdf = std::make_unique<BRDF>(BRDF{
      .pdf = std::bind(&Specular::pdf, std::placeholders::_1,
                       std::placeholders::_2),

      .f =
          [index](const Vector3f &normal, const Vector3f &wi,
                  const Vector3f &wo) mutable {
            Vector3f n(normal);
            Float f = n * wi;
            if (f > 0) {
              index = 1 / index;
              n = -n;
              f = -f;
            }
            Float R0 = (index - 1) * (index - 1) / ((index + 1) * (index + 1));
            return R0 + (1 - R0) * std::pow(1 - std::abs(wi * n), 5);
          },
      .type = []() { return BRDF::Type::Specular; },
  });
  ret.btdf = std::make_unique<BTDF>(BTDF{
      .pdf =
          [index](const Vector3f &normal, const Vector3f &wi) mutable {
            Vector3f n(normal);
            Float f = n * wi;
            if (f > 0) {
              index = 1 / index;
              n = -n;
              f = -f;
            }
            if ((1 - (f * f)) / (index * index) > 1) {
              return std::make_tuple(0.f, Vector3f());
            }
            // assumes normal points outside
            Vector3f wo = (wi - f * n) * (1 / index) -
                          n * std::sqrt(1 - ((1 - f * f) / (index * index)));
            assert(!wo.HasNaNs());
            return std::make_tuple(1.f, wo.Normalize());
          },
      .f =
          [index](const Vector3f &normal, const Vector3f &wi,
                  const Vector3f &wo) mutable {
            Vector3f n(normal);
            Float f = n * wi;
            if (f > 0) {
              index = 1 / index;
              n = -n;
              f = -f;
            }
            Float R0 = (index - 1) * (index - 1) / ((index + 1) * (index + 1));
            return 1 - (R0 + (1 - R0) * std::pow(1 - std::abs(wi * n), 5));
          },
  });
  return ret;
}
}
