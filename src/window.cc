// Copyright (c) 2016 Kai Luo. All rights reserved.

#include <chrono>
#include <thread>

#include "logging.hh"
#include "window.hh"

namespace zLi {

const float Window::FPSLimit = 120;

Window::Window(int w, int h)
    : width_(w), height_(h), disp_(nullptr), should_close_(false) {}
Window::~Window() {
  if (disp_) {
    XDestroyWindow(disp_, window_);
    XCloseDisplay(disp_);
  }
}

std::string Window::ToXColorName(const RGBColor &rgb) {
  char buf[512];
  std::snprintf(buf, sizeof(buf), "RGBi:%f/%f/%f", rgb.r, rgb.g, rgb.b);
  return std::string(buf);
}

std::string Window::ToXColorName(const xyYColor &xyY) {
  char buf[512];
  std::snprintf(buf, sizeof(buf), "CIExyY:%f/%f/%f", xyY.x, xyY.y, xyY.Y);
  return std::string(buf);
}

void Window::Flush() { XFlush(disp_); }

Result<void> Window::Init() {
  disp_ = XOpenDisplay(nullptr);
  if (!disp_) {
    return Error("Can't open display");
  }

  screen_ = DefaultScreen(disp_);
  window_ = XCreateSimpleWindow(disp_, RootWindow(disp_, screen_), 0, 0, width_,
                                height_, 0, BlackPixel(disp_, screen_),
                                BlackPixel(disp_, screen_));
  Atom deleteWindow = XInternAtom(disp_, "WM_DELETE_WINDOW", false);
  XSetWMProtocols(disp_, window_, &deleteWindow, 1);
  XSelectInput(disp_, window_, ExposureMask | KeyPressMask);

  // XGCValues values;
  // unsigned long valuemask = GCCapStyle | GCJoinStyle;
  gc_ = DefaultGC(disp_, screen_);
  XMapWindow(disp_, window_);
  return Ok();
}

void Window::PollEvents() {
  int count = XPending(disp_);
  while (count--) {
    XEvent ev;
    XNextEvent(disp_, &ev);
    // ESC pressed
    if (ev.type == KeyPress && ev.xkey.keycode == 9) {
      should_close_ = true;
    }
    if (ev.type == ClientMessage) {
      should_close_ = true;
    }
  }
}

void Window::Loop(std::function<void()> &&display,
                  std::function<void()> &&atExitLoop) {
  float time_slice = 1 / FPSLimit;
  auto start = std::chrono::high_resolution_clock::now();
  while (!should_close_) {
    PollEvents();
    display();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> diff = end - start;
    if (diff.count() < time_slice) {
      std::this_thread::sleep_for(
          std::chrono::duration<float>(time_slice - diff.count()));
    }
    start = std::chrono::high_resolution_clock::now();
  }
  if (atExitLoop) {
    atExitLoop();
  }
}

}  // namespace zLi
