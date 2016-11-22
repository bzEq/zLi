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
  Film(int width, int height) : width_(width), height_(height) {
    data_.resize(width_);
    for (int i = 0; i < width_; ++i) {
      data_[i].resize(height_);
    }
  }
  Film(const Film &);
  Film(Film &&) = default;
  void Set(int i, int j, Spectrum &&s);
  Result<void> WriteEXR(const std::string &);

private:
  int width_, height_;
  std::vector<std::vector<std::unique_ptr<Spectrum>>> data_;
  void FillRgba(Imf::Rgba *);
};

} // namespace zLi
#endif
