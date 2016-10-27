#ifndef _ZLI_FILM_HH_
#define _ZLI_FILM_HH_
#include "math.hh"
#include "spectrum.hh"

#include <vector>


namespace zLi {
struct Film {
  int w, h;
  std::vector< std::vector<Spectrum>  > film;
  Film(int width, int height): w(width), h(height) {
    film.resize(w);
    for (int i = 0; i < w; ++i) {
      film[i].resize(h);
    }
  }
  std::vector<Spectrum>& operator[](int i) {
    assert(i >= 0 && i < w);
    return film[i];
  }
};

} // namespace zLi
#endif
