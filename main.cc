#include "logging.hh"
#include "renderer.hh"
#include "window.hh"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <cstdio>
#include <cstdlib>
#include <string>

namespace po = boost::program_options;

int main(int argc, char *argv[]) {
  DEBUG("main program started...");
  po::options_description desc("Usage:");
  desc.add_options()("help", "help message")("scene", po::value<std::string>(),
                                             "scene json file");
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);
  if (vm.count("help") || !vm.count("scene")) {
    std::cout << desc << std::endl;
    exit(1);
  }
  std::string scene(vm["scene"].as<std::string>());
  const int w = 1280, h = 720;
  zLi::Renderer rd(scene, w, h);
  zLi::Window window(w, h);
  auto res = window.Init();
  if (!res) {
    ERROR(res.Error().c_str());
    exit(1);
  }
  std::thread rt([&rd]() {
    auto res = rd.Render();
    if (!res) {
      DEBUG(res.Error().c_str());
      return;
    }
  });
  auto chan = rd.RGBChan();
  window.Loop(
      [chan, &window]() {
        while (!chan->Closed()) {
          auto res = chan->Popi();
          if (!res) {
            break;
          }
          auto some(std::move(*res));
          window.FillPixel(some.x, some.y, some.rgb);
        }
      },
      [&rd]() { rd.Stop(); });
  rt.join();
  DEBUG("main program exiting...");
  return 0;
}
