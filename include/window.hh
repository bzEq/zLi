#ifndef _ZLI_WINDOW_HH_
#define _ZLI_WINDOW_HH_
#include "color.hh"
#include "result.hh"

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <cstdio>
#include <cstdlib>
#include <string>

namespace zLi {
class Window {
public:
  static const float FPSLimit;
  Window(int w, int h);
  Result<void> Init();
  void Loop(std::function<void()> &&,
            std::function<void()> &&atExitLoop = nullptr);
  template <typename ColorType>
  void DrawPoint(int x, int y, const ColorType &c) {
    std::string name = ToXColorName(c);
    XColor screen, exact;
    XAllocNamedColor(disp_, ::XDefaultColormap(disp_, screen_), name.c_str(),
                     &screen, &exact);
    XSetForeground(disp_, gc_, exact.pixel);
    XDrawPoint(disp_, window_, gc_, x, y);
  }
  void Flush();
  ~Window();

private:
  static std::string ToXColorName(const RGBColor &);
  static std::string ToXColorName(const xyYColor &);
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
