#ifndef BUTTON_FUNCTIONS_CPP
#define BUTTON_FUNCTIONS_CPP

#include "ButtonFunctions.hpp"

void set_show_timer(Fl_Widget* w, void* DisplayOptions_v) {
  DisplayOptions* d = ((DisplayOptions*)(DisplayOptions_v));
  d->dispTimer = 1;
  d->dispClock = 0;
  d->dispStopwatch = 0;
}

void set_show_clock(Fl_Widget* w, void* DisplayOptions_v) {
  DisplayOptions* d = ((DisplayOptions*)(DisplayOptions_v));
  d->dispTimer = 0;
  d->dispClock = 1;
  d->dispStopwatch = 0;
}

void set_show_stopwatch(Fl_Widget* w, void* DisplayOptions_v) {
  DisplayOptions* d = ((DisplayOptions*)(DisplayOptions_v));
  d->dispTimer = 0;
  d->dispClock = 0;
  d->dispStopwatch = 1;
}

void toggle_seconds(Fl_Widget* w, void* DisplayOptions_v) {
  DisplayOptions* d = ((DisplayOptions*)(DisplayOptions_v));
  d->seconds = !(d->seconds);
}

void reset_stopwatch(Fl_Widget* w, void* DisplayOptions_v) {
  DisplayOptions* d = ((DisplayOptions*)(DisplayOptions_v));
  d->timerStart = std::chrono::system_clock::now();
}

void toggle_mode24hour(Fl_Widget* w, void* DisplayOptions_v) {
  DisplayOptions* d = ((DisplayOptions*)(DisplayOptions_v));
  d->mode24hr = !d->mode24hr;
}

void set_timer(Fl_Widget* w, void* setTimerData_v) {
  Fl_Input* timerStrInput = *(((setTimerData*)setTimerData_v)->timerStrInput);
  DisplayOptions* displayOptions =
      (((setTimerData*)setTimerData_v)->displayOptions);
  int tHour = 0;
  int tMin = 0;
  int tSec = -1;
  sscanf_s(timerStrInput->value(), "%i:%i:%i", &tHour, &tMin, &tSec);
  if (tSec == -1) {
    tHour = 0;
    sscanf_s(timerStrInput->value(), "%i:%i", &tMin, &tSec);
  }
  using namespace std::literals;
  displayOptions->timerEnd =
      std::chrono::system_clock::now() + tHour * 1h + tMin * 1min + tSec * 1s;
  displayOptions->alarmPlayed = 0;
  return;
}

#endif
