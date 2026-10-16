#pragma once

#include "raygui.h"
#include <cstdint>
#include <raylib.h>
#include <vector>

namespace gui {
enum class element_type : uint32_t {
  _BUTTON = 1 << 0,
  _LABEL = 1 << 1,
  _WINDOW = 1 << 2,
  _SLIDER = 1 << 3,
  _PANEL = 1 << 4
};

struct element {
  Vector2 size;

  union {
    element_type e_type;
    struct {
      Rectangle bounds = {100, 100, 0, 0};
      const char *text = "text";
    } button;
    struct {
      Rectangle bounds = {100, 100, 0, 0};
      const char *text = "text";
    } label;

    struct {
      Rectangle bounds = {100, 100, 0, 0};
      const char *text = "text";
    } window;

    struct {
      Rectangle bounds = {100, 100, 0, 0};
      float min = 0;
      float max = 100;
      float *current_val = nullptr;
      const char *left_text = "text";
      const char *text_right = "text";
    } slider;

    struct {
      Rectangle bounds = {100, 100, 0, 0};
      const char *text = "text";
    } panel;
  };
};

// TODO: Add logic to these components and define their behavior.
inline void draw_window(element &e) {
  GuiWindowBox(e.window.bounds, e.window.text);
}
inline void draw_panel(element &e) { GuiPanel(e.panel.bounds, e.panel.text); }
inline void draw_slider(element &e) {
  GuiSlider(e.slider.bounds, e.slider.left_text, e.slider.text_right,
            e.slider.current_val, e.slider.min, e.slider.max);
}
inline void draw_label(element &e) { GuiLabel(e.label.bounds, e.label.text); }
inline void draw_button(element &e) {
  GuiButton(e.button.bounds, e.button.text);
}

struct component {
  // Pre-instantiate the elements upon referencing the struct element.
  inline void check_element(element &e) {

    // Tagged union evaluation reads element reference.
    switch (e.e_type) {
    case element_type::_BUTTON:
      draw_button(e);
      break;

    case element_type::_LABEL:
      draw_label(e);
      break;

    case element_type::_PANEL:
      draw_panel(e);
      break;
    case element_type::_WINDOW:
      draw_panel(e);
      break;
    case element_type::_SLIDER:
      draw_slider(e);
      break;
    }
  }
  inline int check_state(int state, element_type e_type) {
    switch (GuiGetState()) {
    case STATE_NORMAL:
      state = STATE_NORMAL;
      return (state << 4);
      break;
    case STATE_FOCUSED:
      state = STATE_FOCUSED;
      return (state << 4) | static_cast<int>(e_type);
    case STATE_PRESSED:
      state = STATE_PRESSED;
      return (state << 4) | static_cast<int>(e_type);
    case STATE_DISABLED:
      state = STATE_DISABLED;
      return (state << 4) | static_cast<int>(e_type);
    }
    return STATE_NORMAL;
  }
};
} // namespace gui

namespace gui::manager {

template <size_t CAPACITY> struct widget_manager {
  // Manages the element_buffer array.
  gui::element element_buffer[CAPACITY];
  gui::element_type e_type;
};

} // namespace gui::manager
