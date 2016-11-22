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
class Film {
public:
  std::vector<std::vector<std::unique_ptr<Spectrum>>> film;
  Film(int width, int height) : width_(width), height_(height) {
    film.resize(width_);
    for (int i = 0; i < width_; ++i) {
      film[i].resize(height_);
    }
  }
  std::vector<std::unique_ptr<Spectrum>> &operator[](int i) {
    assert(i >= 0 && i < width_);
    return film[i];
  }
  Result<void> WriteEXR(const std::string &);

private:
  int width_, height_;
  void FillRgba(Imf::Rgba *);
};

} // namespace zLi
#endif
