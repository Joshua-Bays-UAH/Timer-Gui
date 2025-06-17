#ifndef DISPLAY_WINDOW_HPP
#define DISPLAY_WINDOW_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <string>

struct DisplayOptions {
  bool dispTimer = 0;
  bool dispClock = 1;
  bool dispStopwatch = 0;
  bool seconds = 0;
  bool alarmPlayed = 0;
  bool mode24hr = 1;
  std::chrono::time_point<std::chrono::system_clock> timerStart;
  std::chrono::time_point<std::chrono::system_clock> timerEnd;
};

void sdl_thread(DisplayOptions* displayOptions);

#endif