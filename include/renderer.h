// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef __ZLI_RENDERER_H_
#define __ZLI_RENDERER_H_
#include <atomic>
#include <memory>
#include <string>

#include "chan.h"
#include "film.h"
#include "filter.h"
#include "option.h"
#include "scene.h"
#include "spectrum.h"

namespace zLi {
class Renderer {
public:
  struct RenderResult {
    int x, y;
    xyYColor xyY;
  };
  Renderer(const std::string &scene_file, int film_width, int film_height,
           int spp = 128);
  Result<void> Render();
  void Stop();
  bool Stopped();
  // std::shared_ptr<Chan<RenderResult>> RGBChan() { return rgb_chan_; }
  std::shared_ptr<Chan<RenderResult>> xyYChan() { return xyY_chan_; }
  Film MoveRenderResult();
  ~Renderer();

private:
  static const Float SampleRadius;
  // void AddToRGBChan(int, int, const Spectrum &);
  void AddToxyYChan(int, int, const Spectrum &);
  Result<void> SlowRender();
  Result<void> ParallelRender();
  void Work(int, int);
  std::unique_ptr<Film> film_;
  std::unique_ptr<Scene> scene_;
  std::string scene_file_;
  int film_width_, film_height_;
  std::atomic<int> render_job_;
  std::atomic<bool> stopped_;
  Spectrum SampleSpectrumAt(Float, Float);
  int spp_;  // samples per pixel
  // std::shared_ptr<Chan<RenderResult>> rgb_chan_;
  std::shared_ptr<Chan<RenderResult>> xyY_chan_;
  filter::Gauss1D filter_;
};
}  // namespace zLi

#endif
