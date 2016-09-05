#ifndef _ZLI_GUI_WINDOW_H_
#define _ZLI_GUI_WINDOW_H_
#include "geometry.h"

#include <xcb/xcb.h>
#include <iostream>
#include <string>


namespace zLi {
namespace gui {

class Window {
public:
  Window(const std::string&, const int, const int);
  void RenderLoop();
  ~Window();
private:
  void HandleEvent(xcb_generic_event_t*);
  xcb_connection_t *connection_ ;
  xcb_screen_t *screen_;
  xcb_window_t win_;
  bool quit_;
  unsigned long frame_counter_;
  xcb_intern_atom_reply_t *atom_wm_delete_window_;  
};

} // end namespace zLi::gui
} // end namespace zLi

#endif
