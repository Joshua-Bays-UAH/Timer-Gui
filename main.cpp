#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Simple_Counter.H>
#include <FL/Fl_Window.H>

#include "ButtonFunctions.hpp"
#include "DisplayWindow.hpp"

#include <cstdio>
#include <thread>
#undef main

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Could not initalize SDL2\n");
    return 1;
  }
  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    printf("Could not initalize SDL2\n");
    return 1;
  }
  if (TTF_Init() < 0) {
    printf("Could not initalize SDL2\n");
    return 1;
  }

  // SDL_ShowCursor(0);
  DisplayOptions displayOptions;
  using namespace std::literals;
  displayOptions.timerStart = std::chrono::system_clock::now();
  displayOptions.timerEnd = (std::chrono::system_clock::now() + 2h + 1min + 5s);

  std::thread renderThread(sdl_thread, &displayOptions);
  renderThread.detach();

  Fl_Window* window = new Fl_Window(640, 300, "Timer Controls");
  Fl_Button* showTimerButton = new Fl_Button(10, 20, 200, 60, "Show Timer");
  showTimerButton->callback(set_show_timer, (void*)&displayOptions);
  Fl_Button* showStopwatchButton =
      new Fl_Button(220, 20, 200, 60, "Show Stopwatch");
  showStopwatchButton->callback(set_show_stopwatch, (void*)&displayOptions);
  Fl_Button* showClockButton = new Fl_Button(430, 20, 200, 60, "Show Clock");
  showClockButton->callback(set_show_clock, (void*)&displayOptions);
  Fl_Button* toggleSecondsButton =
      new Fl_Button(10, 100, 200, 60, "Toggle Seconds");
  toggleSecondsButton->callback(toggle_seconds, (void*)&displayOptions);
  Fl_Button* resetStopwatchButton =
      new Fl_Button(220, 100, 200, 60, "Reset Stopwatch");
  resetStopwatchButton->callback(reset_stopwatch, (void*)&displayOptions);

  Fl_Button* toggleMode24hourButton =
      new Fl_Button(430, 100, 200, 60, "Toggle 12/24 Hour");
  toggleMode24hourButton->callback(toggle_mode24hour, (void*)&displayOptions);

  Fl_Button* setTimerButton = new Fl_Button(10, 220, 200, 60, "Set Timer");
  Fl_Input* timerStrInput = new Fl_Input(10, 180, 100, 30);
  timerStrInput->value("HH:MM:SS");
  setTimerButton->callback(
      set_timer, (void*)(new setTimerData{&timerStrInput, &displayOptions}));

  window->show(argc, argv);
  return Fl::run();
}
