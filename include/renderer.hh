#ifndef __ZLI_RENDERER_HH_
#define __ZLI_RENDERER_HH_
#include "film.hh"
#include "scene.hh"
#include "spectrum.hh"

#include <atomic>
#include <memory>

namespace zLi {
class Renderer {
public:
  Renderer(const std::string &sceneFile, int filmWidth, int filmHeight, int spp,
           Float sampleRadius);
  int Render();
  void Stop();
  bool Stopped();

private:
  int SlowRender();
  int ParallelRender();
  void Work(int, int);
  std::unique_ptr<Film> film_;
  std::unique_ptr<Scene> scene_;
  std::string scene_file_;
  int film_width_, film_height_;
  std::atomic<int> render_job_;
  std::atomic<bool> stopped_;
  Spectrum SampleSpectrumAt(Float, Float);
  Float sample_radius_;
  int spp_; // samples per pixel
};
}

#endif
