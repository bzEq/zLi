// Copyright (c) 2016 Kai Luo <gluokai@gmail.com>. All rights reserved.

#include "light.hh"

namespace zLi {

Spectrum PointLight::Le(const RaySurfaceIntersection &ri) {
  Spectrum le(0);
  Vector3f p = ri.ray(ri.t);
  Geometry g = ri.g;
  Vector3f wi = (position_ - p).Normalize();
  Vector3f wo = -ri.ray.d;
  BSDF bsdf = g.bsdf();
  if (bsdf.brdf) {
    if (bsdf.brdf->type() == BRDF::Type::Specular) {
      auto n = g.Normal(p);
      n = wi * n <= 0 ? n : -n;
      auto rfl = bsdf.brdf->pdf(n, wi);
      Float pdf = std::get<0>(rfl);
      if (pdf > 0 && (std::get<1>(rfl) - wo).Length() < EPSILON) {
        le += le_;
      }
    } else if (bsdf.brdf->type() == BRDF::Type::Diffuse) {
      auto n = g.Normal(p);
      auto rfl = bsdf.brdf->pdf(n, wi);
      Float pdf = std::get<0>(rfl);
      if (pdf > 0) {
        Float f = bsdf.brdf->f(n, wi, wo);
        le += le_ * f * std::abs(wi * n);
      }
    }
  }
  return le;
}

Light PointLight::ImplLight() {
  return Light{
      .Le =
          std::bind(&PointLight::Le, shared_from_this(), std::placeholders::_1),
  };
}

}  // namespace zLi
