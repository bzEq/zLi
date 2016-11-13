#include "renderer.hh"
#include "integrator.hh"
#include "logging.hh"
#include "math.hh"
#include "scene.hh"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <atomic>
#include <thread>

namespace zLi {

const Float Renderer::SampleRadius = 1.f;
Renderer::Renderer(const std::string &sceneFile, int filmWidth, int filmHeight,
                   int spp)
    : scene_file_(sceneFile), film_width_(filmWidth), film_height_(filmHeight),
      render_job_(0), stopped_(false), spp_(spp) {
  rgb_chan_ = std::make_shared<Chan<RenderResult>>();
}

Spectrum Renderer::SampleSpectrumAt(Float x, Float y) { return Spectrum(0); }

void Renderer::AddToRGBChan(int i, int j, const Spectrum &s) {
  rgb_chan_->Push(RenderResult{
      .x = i, .y = j, .rgb = s.ToRGB().Clamp(),
  });
}

void Renderer::Work(int i, int j) {
  Spectrum res(0);
  for (int i = 0; i < spp_; ++i) {
    Float x = i + SampleRadius * (UniformSample() - 0.5);
    Float y = j + SampleRadius * (UniformSample() - 0.5);
    Spectrum s = SampleSpectrumAt(x, y);
    Float r = std::sqrt((x - i) * (x - i) + (y - j) * (y - j));
    s *= filter_.f(r) * ((Float)1 / spp_);
    res += s;
  }
  AddToRGBChan(i, j, res);
  // add to film
  (*film_)[i][j] = std::make_unique<Spectrum>(std::move(res));
}

void Renderer::Stop() { stopped_.store(true); }

bool Renderer::Stopped() { return stopped_; }

Result<void> Renderer::SlowRender() {
  int n = render_job_++;
  while (!stopped_ && n < film_width_ * film_height_) {
    INFO("start working on No.%d job", n);
    Work(n / film_height_, n % film_height_);
    n = render_job_++;
  }
  stopped_.store(true);
  return Ok();
}

Result<void> Renderer::ParallelRender() {
  std::vector<std::thread> workers;
  for (unsigned i = 0; i < std::thread::hardware_concurrency(); ++i) {
    workers.push_back(std::thread([this] {
      while (!stopped_) {
        int n = render_job_++;
        INFO("start working on No.%d job", n);
        if (n >= film_width_ * film_height_) {
          break;
        }
        Work(n / film_height_, n % film_height_);
      }
    }));
  }
  for (auto &w : workers) {
    w.join();
  }
  stopped_.store(true);
  return Ok();
}

Result<void> Renderer::Render() {
  auto res = Scene::SceneFromJson(scene_file_);
  if (!res) {
    DEBUG("can't load scene file: %s", scene_file_.c_str());
    return FormatError("can't load scene file: %s", scene_file_.c_str());
  }
  scene_ = std::make_unique<Scene>(std::move(*res));
  film_ = std::make_unique<Film>(film_width_, film_height_);
  // return ParallelRender();
  return SlowRender();
}

Renderer::~Renderer() {}

} // namespace zLi
