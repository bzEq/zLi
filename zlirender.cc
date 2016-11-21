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
  zLi::SetLogLevel(zLi::DEBUG);
  DEBUG("main program started...");
  po::options_description desc("Usage:");
  desc.add_options()("help", "help message")("scene", po::value<std::string>(),
                                             "scene json file")(
      "width", po::value<int>(), "film width")("height", po::value<int>(),
                                               "film height");
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);
  if (vm.count("help") || !vm.count("scene") || !vm.count("width") ||
      !vm.count("height")) {
    std::cerr << desc << std::endl;
    exit(1);
  }
  std::string scene(vm["scene"].as<std::string>());
  const int w = vm["width"].as<int>(), h = vm["height"].as<int>();
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
    DEBUG("render finished");
  });
  auto chan = rd.xyYChan();
  window.Loop(
      [chan, &window]() {
        static const int EventProcessLimits = 128;
        int count = std::min((int)chan->Size(), EventProcessLimits);
        while (count--) {
          auto res = chan->Popi();
          if (!res) {
            break;
          }
          auto some(std::move(*res));
          window.DrawPoint(some.x, some.y, some.xyY);
        }
        window.Flush();
      },
      [&rd]() { rd.Stop(); });
  rt.join();
  DEBUG("main program exiting...");
  return 0;
}
