#include "gui/window.h"

#include <xcb/xcb.h>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <string>


namespace zLi {
namespace gui {

Window::Window(const std::string& title, const int width, const int height)
  : connection_(nullptr), screen_(nullptr), quit_(false), frame_counter_(0), atom_wm_delete_window_(nullptr) {
  const xcb_setup_t *setup;
  xcb_screen_iterator_t iter;
  int scr;

  // connect to X server
  connection_ = xcb_connect(nullptr, &scr);
  if (!connection_) {
    throw std::runtime_error("could not connect to X server!");
  }
  setup = xcb_get_setup(connection_);
  iter = xcb_setup_roots_iterator(setup);
  while (scr-- > 0)
    xcb_screen_next(&iter);
  screen_ = iter.data;

  // setup window
  win_ = xcb_generate_id(connection_);
  
  uint32_t value_mask, value_list[32];
  value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  value_list[0] = screen_->black_pixel;
  value_list[1] =
    XCB_EVENT_MASK_KEY_RELEASE |
    XCB_EVENT_MASK_KEY_PRESS |
    XCB_EVENT_MASK_EXPOSURE |
    XCB_EVENT_MASK_STRUCTURE_NOTIFY |
    XCB_EVENT_MASK_POINTER_MOTION |
    XCB_EVENT_MASK_BUTTON_PRESS |
    XCB_EVENT_MASK_BUTTON_RELEASE;
  
  xcb_create_window(connection_,
                    XCB_COPY_FROM_PARENT,
                    win_, screen_->root,
                    0, 0, width, height, 0,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen_->root_visual,
                    value_mask, value_list);

  /* Magic code that will send notification when window is destroyed */
  xcb_intern_atom_cookie_t cookie = xcb_intern_atom(connection_, 1, 12, "WM_PROTOCOLS");
  xcb_intern_atom_reply_t* reply = xcb_intern_atom_reply(connection_, cookie, 0);

  xcb_intern_atom_cookie_t cookie1 = xcb_intern_atom(connection_, 0, 16, "WM_DELETE_WINDOW");
  atom_wm_delete_window_ = xcb_intern_atom_reply(connection_, cookie1, 0);

  xcb_change_property(connection_, XCB_PROP_MODE_REPLACE,
                      win_, (*reply).atom, 4, 32, 1,
                      &(*atom_wm_delete_window_).atom);

  xcb_change_property(connection_, XCB_PROP_MODE_REPLACE,
                      win_, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
                      title.size(), title.c_str());

  free(reply);

  xcb_map_window(connection_, win_);
};

Window::~Window() {
  if (connection_) {
    xcb_destroy_window(connection_, win_);
    xcb_disconnect(connection_);
    connection_ = nullptr;
  }
}

void Window::RenderLoop() {
  xcb_flush(connection_);
  while (!quit_) {
    auto start = std::chrono::high_resolution_clock::now();
    xcb_generic_event_t *event;
    while ((event = xcb_poll_for_event(connection_))) {
      HandleEvent(event);
      free(event);
    }
    // render();
    frame_counter_++;
  }
}

void Window::HandleEvent(xcb_generic_event_t* event) {
  switch (event->response_type & 0x7f) {
    case XCB_CLIENT_MESSAGE:
      if ((*(xcb_client_message_event_t*)event).data.data32[0] ==
          (*atom_wm_delete_window_).atom) {
        quit_ = true;
      }
      break;
    default:
      break;
  }
}

} // end namespace zLi::gui
} // end namespace zLi

