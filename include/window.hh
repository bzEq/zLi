#ifndef _ZLI_WINDOW_HH_
#define _ZLI_WINDOW_HH_
#include "color.hh"
#include "result.hh"

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <cstdio>
#include <cstdlib>

namespace zLi {
class Window {
public:
  static const float FPSLimit;
  Window(int w, int h);
  Result<void> Init();
  void Loop(std::function<void()> &&,
            std::function<void()> &&atExitLoop = nullptr);
  void DrawPoint(int, int, const RGBColor &);
  void Flush();
  ~Window();

private:
  void PollEvents();
  int width_, height_;
  ::Display *disp_;
  int screen_;
  ::Window window_;
  ::GC gc_;
  bool should_close_;
};

} // zLi
#endif
