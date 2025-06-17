#ifndef BUTTON_FUNCTIONS_HPP
#define BUTTON_FUNCTIONS_HPP

#include <FL/Fl_Input.H>
#include <FL/Fl_Widget.H>
#include "DisplayWindow.hpp"

struct setTimerData {
  Fl_Input** timerStrInput;
  DisplayOptions* displayOptions;
};

void toggle_seconds(Fl_Widget* w, void* DisplayOptions_v);
void set_show_timer(Fl_Widget* w, void* DisplayOptions_v);
void set_show_clock(Fl_Widget* w, void* DisplayOptions_v);
void set_show_stopwatch(Fl_Widget* w, void* DisplayOptions_v);
void reset_stopwatch(Fl_Widget* w, void* DisplayOptions_v);
void toggle_mode24hour(Fl_Widget* w, void* DisplayOptions_v);
void set_timer(Fl_Widget* w, void* setTimerData_v);

#endif
