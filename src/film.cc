#include "film.hh"
#include "color.hh"
#include "result.hh"

#include <OpenEXR/ImfRgba.h>
#include <OpenEXR/ImfRgbaFile.h>

namespace zLi {

void Film::FillRgba(Imf::Rgba *pixels) {
  for (int i = 0; i < width_; ++i) {
    for (int j = 0; j < height_; ++j) {
      if (film[i][j]) {
        RGBColor rgb(film[i][j]->ToRGB());
        int k = i * height_ + j;
        pixels[k].r = rgb.r;
        pixels[k].g = rgb.g;
        pixels[k].b = rgb.b;
        pixels[k].a = 0;
      }
    }
  }
}

Result<void> Film::WriteEXR(const std::string &exr) {
  Imf::Rgba *pixels = new Imf::Rgba[width_ * height_];
  FillRgba(pixels);
  try {
    Imf::RgbaOutputFile file(exr.c_str(), width_, height_, Imf::WRITE_RGBA);
    file.setFrameBuffer(pixels, height_, 1);
    file.writePixels(height_);
  } catch (const std::exception &e) {
    delete[] pixels;
    return Error(e.what());
  }
  delete[] pixels;
  return Ok();
}
}
