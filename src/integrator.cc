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
    auto pos = (*ri).ray((*ri).t);
    // DEBUG("bounce %d, intersection found at position (%f, %f, %f)", i, pos.x,
    //       pos.y, pos.z);

    if (i == 0) {
      L += (*ri).g.Le();
    }
    L += F * scene.DirectLight(*ri);
    // bsdf
    auto bsdf((*ri).g.bsdf());
    if (bsdf.type() == BSDF::Type::Specular) {
      // specular
      auto n = (*ri).g.Normal(ray((*ri).t));
      n = ray.d * n <= 0 ? n : -n;
      auto rfl = bsdf.pdf(n, ray.d);
      ray = (*ri).SpawnRay(std::get<1>(rfl));
    } else if (bsdf.type() == BSDF::Type::Diffuse) {
      // diffuse
      auto n = (*ri).g.Normal(ray((*ri).t));
      auto rfl = bsdf.pdf(n, ray.d);
      Float pdf = std::get<0>(rfl);
      if (pdf <= 0) {
        break;
      }
      auto wo = std::get<1>(rfl);
      Float f = bsdf.f(n, ray.d, wo);
      F *= f * std::abs(n * wo) / pdf;
      ray = (*ri).SpawnRay(wo);
    } else if (bsdf.type() == BSDF::Type::Refractive) {
      // refractive
    } else {
      WARN("no such bsdf type, %d", bsdf.type());
      continue;
    }
    // russian roulette
    if (i > 3) {
      Float q = F.ToXYZ().y;
      if (UniformSample() <= q) {
        break;
      }
      F /= 1 - q;
    }
  }
  return L;
}

} // end namespace zLi
