#include "math.hh"
#include "logging.hh"
#include "renderer.hh"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <thread>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>


class zLiWindow: public Fl_Window {
public:
  zLiWindow(int w, int h): Fl_Window(w, h) {}
};

int RunGui(int width, int height) {
  INFO("gui thread launced");
  zLiWindow window(width, height);
  window.end();
  window.show();
  return Fl::run();
}

int Render(const std::string& file, int width, int height) {
  INFO("render thread launced");
  zLi::Renderer r(file, width, height);
  int ret = r.Render();
  INFO("render thread exiting");
  return ret;
}

int main(int argc, char* argv[])
{
  if (argc != 4) {
    fprintf(stderr, "usage: %s <scene.json> <width> <height>\n", argv[0]);
    exit(1);
  }
  zLi::SetLogLevel(zLi::INFO);
  int width = atoi(argv[2]);
  int height = atoi(argv[3]);

  auto gui = std::thread(RunGui, width, height);

  auto render = std::thread(Render, argv[1], width, height);

  render.join();
  gui.join();
  return 0;
}
