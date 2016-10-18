#ifndef __ZLI_RENDERER_HH_
#define __ZLI_RENDERER_HH_
#include "film.hh"
#include "scene.hh"

#include <memory>
#include <atomic>


namespace zLi {
class Renderer {
public:
  Renderer(const std::string& sceneFile,
           int filmWidth,
           int filmHeight);
  int Render();
private:
  int ParallelRender();
  void Work(int, int);
  std::unique_ptr<Film> film_;
  std::unique_ptr<Scene> scene_;
  std::string scene_file_;
  int film_width_, film_height_;
  std::atomic<int> render_job_;
  std::atomic<bool> stopped_;
};

}

#endif
