#include "window.hh"

int main() {
  zLi::Window w(1280, 720);
  w.Init();
  w.Loop([] { return; });
  return 0;
}
