#ifndef _ZLI_FILM_HH_
#define _ZLI_FILM_HH_
#include "math.hh"
#include "result.hh"
#include "spectrum.hh"

#include <OpenEXR/ImfRgba.h>
#include <OpenEXR/ImfRgbaFile.h>

#include <string>
#include <vector>

namespace zLi {
struct Film {
  int w, h;
  std::vector<std::vector<std::unique_ptr<Spectrum>>> film;
  Film(int width, int height) : w(width), h(height) {
    film.resize(w);
    for (int i = 0; i < w; ++i) {
      film[i].resize(h);
    }
  }
  std::vector<std::unique_ptr<Spectrum>> &operator[](int i) {
    assert(i >= 0 && i < w);
    return film[i];
  }
  Result<void> WriteEXR(const std::string &);
  void FillRgba(Imf::Rgba *);
};

} // namespace zLi
#endif
