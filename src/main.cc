#include "geometry.hh"
#include "logging.hh"

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


class zLiWindow: public Fl_Window {
public:
  zLiWindow(int w, int h): Fl_Window(w, h) {}
};

int RunGui() {
  INFO("gui thread launced");
  zLiWindow window(640, 480);
  window.end();
  window.show();
  return Fl::run();
}

int Render(const std::string& file) {
  INFO("render thread launced");
  boost::property_tree::ptree json;
  boost::property_tree::read_json(file, json);
  auto name = json.get<std::string>("project_name");
  auto version = json.get<std::string>("version");
  INFO("%s-%s", name.c_str(), version.c_str());
  return 0;
}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    fprintf(stderr, "usage: %s <scene.json>\n", argv[0]);
    exit(1);
  }
  zLi::SetLogLevel(zLi::INFO);
  auto gui = std::thread(RunGui);
  auto render = std::thread(Render, argv[1]);
  render.join();
  gui.join();
  return 0;
}
