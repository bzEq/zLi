#include "window.h"

int main() {
  zLi::Window w(1280, 720);
  auto res = w.Init();
  if (!res) {
    std::cout << res.Error() << std::endl;
    exit(1);
  }
  w.Loop([] { return; });
  return 0;
}
