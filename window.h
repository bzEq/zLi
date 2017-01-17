// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef ZLI_WINDOW_H_
#define ZLI_WINDOW_H_
#include <cstdio>
#include <cstdlib>
#include <string>

#include "color.h"
#include "kl/error.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>

namespace zLi {
class Window {
public:
  static const float FPSLimit;
  Window(int w, int h);
  kl::Result<void> Init();
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

}  // namespace zLi
#endif
