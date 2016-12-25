// Copyright (c) 2016 Kai Luo. All rights reserved.

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <cstdio>
#include <cstdlib>
#include <string>

#include "logging.h"
#include "renderer.h"
#include "window.h"

int main(int argc, char *argv[]) {
  zLi::Logger::SetLogLevel(zLi::Logger::DEBUG);
  INFOLOG("main program started...");
  boost::program_options::options_description desc("Usage:");
  desc.add_options()("help", "help message")(
      "scene", boost::program_options::value<std::string>(), "scene json file")(
      "width", boost::program_options::value<int>(), "film width")(
      "height", boost::program_options::value<int>(),
      "film height")("exr", boost::program_options::value<std::string>(),
                     "output openexr file");
  boost::program_options::variables_map vm;
  try {
    boost::program_options::store(
        boost::program_options::parse_command_line(argc, argv, desc), vm);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }
  boost::program_options::notify(vm);
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
    ERRORLOG(res.Error().c_str());
    exit(1);
  }
  std::thread rt([&rd, exr]() {
    auto res = rd.Render();
    if (!res) {
      ERRORLOG(res.Error().c_str());
      return;
    }
    if (!exr.empty()) {
      zLi::Film f(rd.MoveRenderResult());
      f.WriteEXR(exr);
    }
    DEBUGLOG("render finished");
  });
  auto chan = rd.xyYChan();
  window.Loop(
      [chan, &window]() {
        static const int EventProcessLimits = 128;
        int count =
            std::min(static_cast<int>(chan->Size()), EventProcessLimits);
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
  INFOLOG("main program exiting...");
  return 0;
}
