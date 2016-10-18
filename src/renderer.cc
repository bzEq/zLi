#include "renderer.hh"
#include "scene.hh"
#include "logging.hh"
#include "integrator.hh"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <atomic>
#include <thread>

namespace zLi {

Renderer::Renderer(const std::string& sceneFile,
                   int filmWidth, int filmHeight)
  : scene_file_(sceneFile),
    film_width_(filmWidth),
    film_height_(filmHeight),
    render_job_(0),
    stopped_(false) { }

void Renderer::Work(int x, int y) { }

int Renderer::ParallelRender() {
  std::vector<std::thread> workers;
  for (unsigned i = 0; i < std::thread::hardware_concurrency(); ++i) {
    workers.push_back(std::thread([this] {
          while (!stopped_) {
            int n = render_job_++;
            INFO("start working on No.%d job", n);
            if (n >= film_width_ * film_height_) return;
            Work(n / film_height_, n % film_height_);
          }
        })
    );
  }
  for (auto& w: workers) w.join();
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
  return ParallelRender();
}

} // namespace zLi
