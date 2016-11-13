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
  void FillPixel(int, int, const RGBColor &);
  ~Window();

private:
  int width_, height_;
  ::Display *disp_;
  int screen_;
  ::Window window_;
  ::GC gc_;
};

} // zLi
#endif
