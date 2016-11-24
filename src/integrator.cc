// Copyright (c) 2016 Kai Luo. All rights reserved.

#include "integrator.hh"
#include "logging.hh"
#include "scene.hh"
#include "spectrum.hh"

namespace zLi {

Spectrum PathIntegrator::Li(const Scene &scene, const Ray &r, int maxBounces) {
  Spectrum L(0);
  Spectrum F(1);
  Ray ray(r);
  for (int i = 0; i < maxBounces; ++i) {
    auto ri = scene.Intersect(ray);
    if (!ri) {
      break;
    }
    // auto pos = (*ri).ray((*ri).t);
    // DEBUG("bounce %d, intersection found at position (%f, %f, %f)", i, pos.x,
    //       pos.y, pos.z);
    L += F * scene.DirectLight(*ri);
    // bsdf
    auto bsdf = (*ri).g.bsdf();
    if (bsdf.btdf) {
      auto n = (*ri).g.Normal(ray((*ri).t));
      auto rfr = bsdf.btdf->pdf(n, ray.d);
      Float pdf = std::get<0>(rfr);
      if (pdf > 0) {
        auto wo = std::get<1>(rfr);
        ray = (*ri).SpawnRay(wo);
        Spectrum tmp = PathIntegrator::Li(scene, ray, maxBounces - i);
        Float f = bsdf.btdf->f(n, ray.d, wo);
        L += F * (*ri).g.R() * (f * std::abs(n * wo) / pdf) * tmp;
      }
    }
    if (bsdf.brdf) {
      if (bsdf.brdf->type() == BRDF::Type::Specular) {
        auto n = (*ri).g.Normal(ray((*ri).t));
        n = ray.d * n <= 0 ? n : -n;
        auto rfl = bsdf.brdf->pdf(n, ray.d);
        Float pdf = std::get<0>(rfl);
        if (pdf <= 0) {
          break;
        }
        auto wo = std::get<1>(rfl);
        Float f = bsdf.brdf->f(n, ray.d, wo);
        F *= f * std::abs(n * wo) / pdf;
        F *= (*ri).g.R();
        ray = (*ri).SpawnRay(wo);
      } else if (bsdf.brdf->type() == BRDF::Type::Diffuse) {
        auto n = (*ri).g.Normal(ray((*ri).t));
        auto rfl = bsdf.brdf->pdf(n, ray.d);
        Float pdf = std::get<0>(rfl);
        if (pdf <= 0) {
          break;
        }
        auto wo = std::get<1>(rfl);
        Float f = bsdf.brdf->f(n, ray.d, wo);
        F *= f * std::abs(n * wo) / pdf;
        F *= (*ri).g.R();
        ray = (*ri).SpawnRay(wo);
      }
    }
    // russian roulette
    if (i > 3) {
      Float q = F.ToxyY().Y;
      if (UniformSample() <= q) {
        break;
      }
      F /= 1 - q;
    }
  }
  return L;
}

}  // namespace zLi
