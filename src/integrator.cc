#include "integrator.hh"
#include "sample.hh"
#include "scene.hh"
#include "spectrum.hh"

namespace zLi {

Spectrum PathIntegrator::Li(const Scene &scene, const Ray &r, int maxBounces) {
  Spectrum L((Float)0);
  Spectrum F((Float)1);
  Ray ray(r);
  for (int bounces = 0; bounces < maxBounces; bounces++) {
    auto ri = scene.Intersect(ray);
    if (!ri) {
      // TODO: check ray hit light
      if (bounces == 0) {
        L += scene.DirectLight(ray.o);
      }
      break;
    }
    if (bounces == 0) {
      L += (*ri).g.Le();
    }
    L += F * scene.DirectLight(*ri);
    // bsdf
    Vector3f wo = -ray.d;
    Vector3f position = ray((*ri).t);
    Vector3f normal = (*ri).g.Normal(position);
    auto sample = (*ri).g.bsdf().pdf(normal, wo);
    Float pdf = std::get<0>(sample);
    if (pdf <= 0)
      break;
    Vector3f wi = std::get<1>(sample);
    Float f = (*ri).g.bsdf().f(normal, wi, wo);
    F *= f * std::abs(normal * wi) / pdf;
    ray = (*ri).SpawnRay(-wi);
    // russian roulette
    if (bounces > 3) {
      Float q = 0.05;
      if (UniformSample() <= q)
        break;
      F /= 1 - q;
    }
  }
  return L;
}

} // end namespace zLi
