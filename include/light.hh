#ifndef _ZLI_LIGHT_HH_
#define _ZLI_LIGHT_HH_
#include "math.hh"

namespace zLi {

struct Light {
  // @arg: position, emit direction
  std::function<Spectrum(const Vector3f &, const Vector3f &)> Le;
};

} // zLi

#endif
