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
#include <chrono>
#include <thread>

namespace zLi {

const Float Renderer::SampleRadius = 0.5;
Renderer::Renderer(const std::string &sceneFile, int film_width,
                   int film_height, int spp)
    : scene_file_(sceneFile), film_width_(film_width),
      film_height_(film_height), render_job_(0), stopped_(false), spp_(spp) {
  xyY_chan_ = std::make_shared<Chan<RenderResult>>();
}

Spectrum Renderer::SampleSpectrumAt(Float x, Float y) {
  Float l = (Float)std::max(film_width_ + 2 * SampleRadius,
                            film_height_ + 2 * SampleRadius);
  Float tx = (film_width_ + 2 * SampleRadius) / (2 * l);
  Float ty = (film_height_ + 2 * SampleRadius) / (2 * l);
  // normalize
  Float nx = (x + SampleRadius) / l - tx;
  Float ny = ty - (y + SampleRadius) / l;
  // DEBUG("screen axis: (%f,%f), normalized axis: (%f,%f)", x, y, nx, ny);
  Ray ray = scene_->GenerateRay(nx, ny);
  // DEBUG("ray direction: (%f, %f, %f)", ray.d.x, ray.d.y, ray.d.z);
  return PathIntegrator::Li(*scene_, ray);
}

static xyYColor PrimaryColors[] = {
    xyYColor(0.64, 0.33, 0.2126), xyYColor(0.3, 0.6, 0.7152),
    xyYColor(0.15, 0.06, 0.0722),
    xyYColor(0.3127, 0.329, 1.0), // red, green, blue, white
};

void Renderer::AddToxyYChan(int i, int j, const Spectrum &s) {
  xyY_chan_->Push(RenderResult{
      .x = i, .y = j, .xyY = s.ToxyY(),
  });
}

void Renderer::Work(int i, int j) {
  // DEBUG("working on pixel (%d,%d)", i, j);
  Spectrum res(0);
  for (int k = 0; k < spp_; ++k) {
    auto sx = SampleRadius * (2 * UniformSample() - 1);
    auto sy = SampleRadius * (2 * UniformSample() - 1);
    // DEBUG("sample offset (%f, %f)", sx, sy);
    Float x = i + sx;
    Float y = j + sy;
    assert(std::abs(x - i) <= SampleRadius);
    assert(std::abs(y - j) <= SampleRadius);
    // DEBUG("pixel sample (%f, %f)", x, y);
    Spectrum s = SampleSpectrumAt(x, y);
    Float r = std::sqrt((x - i) * (x - i) + (y - j) * (y - j));
    s *= filter_.f(r) * ((Float)1 / spp_);
    res += s;
  }
  AddToxyYChan(i, j, res);
  // add to film
  assert(film_);
  film_->Set(i, j, std::move(res));
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
  auto start = std::chrono::high_resolution_clock::now();
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
  std::chrono::duration<Float> diff =
      std::chrono::high_resolution_clock::now() - start;
  DEBUG("time elapse: %fs", diff.count());
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
  // return SlowRender();
}

Film Renderer::GetRenderResult() { return std::move(*film_); }

Renderer::~Renderer() {}

} // namespace zLi
