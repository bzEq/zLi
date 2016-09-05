#include "gui/window.h"


int main()
{
  using namespace zLi::gui;
  Window window("hello", 1024, 768);
  window.RenderLoop();
  return 0;
}
