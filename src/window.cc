#include "window.hh"

#include <chrono>
#include <thread>

namespace zLi {

const float Window::FPSLimit = 120;

Window::Window(int w, int h) : width_(w), height_(h), disp_(nullptr) {}
Window::~Window() {
  if (disp_) {
    XDestroyWindow(disp_, window_);
    XCloseDisplay(disp_);
  }
}

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

  XGCValues values;
  unsigned long valuemask = GCCapStyle | GCJoinStyle;
  gc_ = XCreateGC(disp_, window_, valuemask, &values);
  if (gc_ < 0) {
    return Error("Can't create GC");
  }
  XMapWindow(disp_, window_);
  return Ok();
}

void Window::Loop(std::function<void()> &&display,
                  std::function<void()> &&atExitLoop) {
  float time_slice = 1 / FPSLimit;
  auto start = std::chrono::high_resolution_clock::now();
  while (true) {
    XEvent ev;
    XNextEvent(disp_, &ev);
    // ESC pressed
    if (ev.type == KeyPress && ev.xkey.keycode == 9) {
      break;
    }
    if (ev.type == ClientMessage) {
      break;
    }
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

} // zLi
