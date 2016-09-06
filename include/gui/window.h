#ifndef _ZLI_GUI_WINDOW_H_
#define _ZLI_GUI_WINDOW_H_
#include "geometry.h"

#include <xcb/xcb.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <string>
#include <vector>


namespace zLi {
namespace gui {

class Window {
public:
  Window(const std::string&, const int, const int);
  void RenderLoop();
  ~Window();
private:
  void HandleEvent(xcb_generic_event_t*);
  void Render();
  void InitVulkan();

  std::string app_name_;
  int fps_max_;

  // xcb related
  xcb_connection_t *connection_ ;
  xcb_screen_t *screen_;
  xcb_window_t win_;
  bool quit_;
  unsigned long frame_counter_;
  xcb_intern_atom_reply_t *atom_wm_delete_window_;  

  // vulkan related
  VkInstance vk_instance_;
  std::vector<VkPhysicalDevice> vk_phy_devs_;
  VkPhysicalDevice vk_phy_dev_;
  VkPhysicalDeviceProperties vk_phy_dev_prop_;
};

} // end namespace zLi::gui
} // end namespace zLi

#endif
