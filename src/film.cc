// Copyright (c) 2016 Kai Luo. All rights reserved.

#include <OpenEXR/ImfRgba.h>
#include <OpenEXR/ImfRgbaFile.h>

#include "color.hh"
#include "film.hh"
#include "result.hh"

namespace zLi {

Film::Film(const Film &f) {
  data_.resize(f.width_);
  for (int i = 0; i < f.width_; ++i) {
    for (int j = 0; j < f.height_; ++j) {
      data_[i].push_back(std::make_unique<Spectrum>(*f.data_[i][j]));
    }
  }
}

void Film::Set(int i, int j, Spectrum &&s) {
  if (i >= 0 && i < width_ && j >= 0 && j < height_) {
    data_[i][j] = std::make_unique<Spectrum>(std::move(s));
  }
}

// wont check size of pixels
void Film::FillRgba(Imf::Rgba *pixels) {
  for (int i = 0; i < width_; ++i) {
    for (int j = 0; j < height_; ++j) {
      if (data_[i][j]) {
        RGBColor rgb(data_[i][j]->ToRGB());
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
}  // namespace zLi
