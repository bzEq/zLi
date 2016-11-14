#include "renderer.hh"
#include "CIE.hh"
#include "color.hh"
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
  xyY_chan_ = std::make_shared<Chan<RenderResult>>();
}

Spectrum Renderer::SampleSpectrumAt(Float x, Float y) {
  Float xs[5] = {380, 480, 580, 680, 780};
  Float ys[5] = {0, 0, 0, 0, 20};
  return Spectrum(xs, ys, sizeof(xs) / sizeof(Float));
}

static xyYColor PrimaryColors[] = {
    xyYColor(0.64, 0.33, 0.2126), xyYColor(0.3, 0.6, 0.7152),
    xyYColor(0.15, 0.06, 0.0722),
    xyYColor(0.3127, 0.329, 1.0), // red, green, blue, white
};

void Renderer::AddToxyYChan(int i, int j, const Spectrum &s) {
  xyY_chan_->Push(RenderResult{
      .x = i,
      .y = j, // .xyY = s.ToxyY(),
      .xyY = xyYColor((Float)i / film_width_,
                      (Float)(film_height_ - j) / film_height_, 0.3),
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
  AddToxyYChan(i, j, res);
  // add to film
  assert(film_);
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
  return ParallelRender();
}

Renderer::~Renderer() {}

} // namespace zLi
