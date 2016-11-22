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
  zLi::Logger::SetLogLevel(zLi::Logger::DEBUG);
  INFO("main program started...");
  po::options_description desc("Usage:");
  desc.add_options()("help", "help message")("scene", po::value<std::string>(),
                                             "scene json file")(
      "width", po::value<int>(), "film width")("height", po::value<int>(),
                                               "film height")(
      "exr", po::value<std::string>(), "output openexr file");
  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }
  po::notify(vm);
  if (vm.count("help") || !vm.count("scene") || !vm.count("width") ||
      !vm.count("height")) {
    std::cerr << desc << std::endl;
    exit(1);
  }
  std::string exr;
  if (vm.count("exr")) {
    exr = vm["exr"].as<std::string>();
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
  std::thread rt([&rd, exr]() {
    auto res = rd.Render();
    if (!res) {
      ERROR(res.Error().c_str());
      return;
    }
    if (!exr.empty()) {
      zLi::Film f(rd.GetRenderResult());
      f.WriteEXR(exr);
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
          zLi::Renderer::RenderResult ans(std::move(*res));
          window.DrawPoint(ans.x, ans.y, ans.xyY);
        }
        window.Flush();
      },
      [&rd]() { rd.Stop(); });
  rt.join();
  INFO("main program exiting...");
  return 0;
}
