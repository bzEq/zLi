#include "renderer.hh"
#include "integrator.hh"
#include "logging.hh"
#include "scene.hh"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <atomic>
#include <thread>

namespace zLi {

const Float Renderer::SampleRadius = 1;
const int Renderer::SPP = 9;

Renderer::Renderer(const std::string &sceneFile, int filmWidth, int filmHeight)
    : scene_file_(sceneFile), film_width_(filmWidth), film_height_(filmHeight),
      render_job_(0), stopped_(false) {}

Spectrum SampleSpectrum(Float x, Float y) { return Spectrum(); }

void Renderer::Work(int i, int j) { Float x = i, y = j; }

void Renderer::Stop() { stopped_.store(true); }

bool Renderer::Stopped() { return stopped_; }

int Renderer::SlowRender() {
  int n = render_job_++;
  while (!stopped_ && n < film_width_ * film_height_) {
    INFO("start working on No.%d job", n);
    Work(n / film_height_, n % film_height_);
    n = render_job_++;
  }
  stopped_.store(true);
  return 0;
}

int Renderer::ParallelRender() {
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
  return 0;
}

int Renderer::Render() {
  auto res = Scene::SceneFromJson(scene_file_);
  if (!res) {
    FATAL("can't load scene file: %s", scene_file_.c_str());
    return -1;
  }
  scene_ = std::make_unique<Scene>(std::move(*res));
  film_ = std::make_unique<Film>(film_width_, film_height_);
  // return ParallelRender();
  return SlowRender();
}

} // namespace zLi
