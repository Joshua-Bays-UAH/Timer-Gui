#ifndef DISPLAY_WINDOW_CPP
#define DISPLAY_WINDOW_CPP

#define _CRT_SECURE_NO_WARNINGS
#include "DisplayWindow.hpp"

#undef main

void sdl_thread(DisplayOptions* displayOptions) {
  SDL_Window* timerWindow =
      SDL_CreateWindow("Timer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       800, 600, SDL_WINDOW_RESIZABLE);
  if (!timerWindow) {
    printf("Could not create window\n");
    return;
  }
  SDL_Renderer* renderer = SDL_CreateRenderer(timerWindow, -1, 0);
  if (!renderer) {
    printf("Cout not create renderer\n");
    return;
  }

  SDL_AudioSpec wavSpec;
  uint32_t wavLength;
  uint8_t* wavBuffer;
  SDL_AudioDeviceID audioDevice;
  SDL_LoadWAV("chime.wav", &wavSpec, &wavBuffer, &wavLength);
  audioDevice = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

  uint8_t r = 0x00;
  uint8_t g = 0x00;
  uint8_t b = 0x00;
  char labelBuff[20];
  char timeBuff[16];
  int i = 0;

  SDL_Event event;
  TTF_Font* timerFont = TTF_OpenFont("RedHat.ttf", 600);
  SDL_Color timerColor = {0x00, 0x77, 0xC8, 0x00};
  std::string timeString = "";
  std::string labelString = "";
  SDL_Texture* timerTexture;
  SDL_Texture* labelTexture;
  SDL_Surface* timerSurface;
  SDL_Surface* labelSurface;

setCoords:
  SDL_Rect timerRect = {0, 0, 0, 0};
  SDL_Rect labelRect = {0, 0, 0, 0};
  SDL_GetWindowSize(timerWindow, &timerRect.w, &timerRect.h);
  timerRect.h /= 2;
  timerRect.y = timerRect.h;
  labelRect.h = (int)(timerRect.h * .6);
  labelRect.w = timerRect.w;
  labelRect.y = timerRect.y - labelRect.h;
  while (1) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          exit(0);
          // SDL_DestroyWindow(timerWindow);
          break;
#if 1
        case SDL_WINDOWEVENT:
          if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            goto setCoords;
          }
          break;
#endif
      }
    }
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    if (displayOptions->dispClock) {
      labelString = "Time of Day";
      std::time_t t = std::chrono::system_clock::to_time_t(
          std::chrono::system_clock::now());
      std::tm* lt = std::localtime(&t);
      int h = lt->tm_hour;
      if (displayOptions->mode24hr) {
        bool am = 1;
        if (h == 0) {
          h = 12;
        } else if (h > 12) {
          h -= 12;
          am = 0;
        }
        snprintf(timeBuff, 6, "%.2i:%.2i", h, lt->tm_min);
        if (displayOptions->seconds) {
          snprintf(timeBuff + 5, 4, ":%.2i", lt->tm_sec);
        }
        snprintf(timeBuff + strnlen(timeBuff, sizeof(timeBuff)), 5, " %s",
                 am ? "AM" : "PM");
      } else {
        if (displayOptions->seconds) {
          snprintf(timeBuff, 10, "%.2i:%.2i:%.2i", h, lt->tm_min, lt->tm_sec);
        } else {
          snprintf(timeBuff, 8, " %.2i:%.2i ", h, lt->tm_min);
        }
      }
      timeString = std::string(timeBuff);
    } else if (displayOptions->dispTimer) {
      labelString = "Time Remaining";
      int d = std::chrono::duration_cast<std::chrono::seconds>(
                  displayOptions->timerEnd - std::chrono::system_clock::now())
                  .count();
      if (d < 0) {
        d = 0;
        if (!displayOptions->alarmPlayed) {
          displayOptions->alarmPlayed = 1;
          SDL_QueueAudio(audioDevice, wavBuffer, wavLength);
          SDL_PauseAudioDevice(audioDevice, 0);
          SDL_Delay(1000);
          // SDL_CloseAudioDevice(audioDevice);
        }
      }
      if (displayOptions->seconds) {
        snprintf(timeBuff, 10, "%.2i:%.2i:%.2i", (int)floor(d / 3600),
                 (int)floor((d % 3600) / 60), d % 60);
      } else {
        snprintf(timeBuff, 8, " %.2i:%.2i ", (int)floor(d / 3600),
                 (int)floor((d % 3600) / 60));
      }
      timeString = std::string(timeBuff);
    } else if (displayOptions->dispStopwatch) {
      labelString = "Elapsed Time";
      int d = std::chrono::duration_cast<std::chrono::seconds>(
                  std::chrono::system_clock::now() - displayOptions->timerStart)
                  .count();
      if (displayOptions->seconds) {
        snprintf(timeBuff, 10, "%.2i:%.2i:%.2i", (int)floor(d / 3600),
                 (int)floor((d % 3600) / 60), d % 60);
      } else {
        snprintf(timeBuff, 8, " %.2i:%.2i ", (int)floor(d / 3600),
                 (int)floor((d % 3600) / 60));
      }
      timeString = std::string(timeBuff);
    }

    timerSurface =
        TTF_RenderText_Solid(timerFont, timeString.c_str(), timerColor);
    timerTexture = SDL_CreateTextureFromSurface(renderer, timerSurface);
    SDL_RenderCopy(renderer, timerTexture, NULL, &timerRect);

    labelSurface =
        TTF_RenderText_Solid(timerFont, labelString.c_str(), timerColor);
    labelTexture = SDL_CreateTextureFromSurface(renderer, labelSurface);
    SDL_RenderCopy(renderer, labelTexture, NULL, &labelRect);

    SDL_RenderPresent(renderer);
    SDL_Delay(100);

    SDL_FreeSurface(timerSurface);
    SDL_DestroyTexture(timerTexture);
    SDL_FreeSurface(labelSurface);
    SDL_DestroyTexture(labelTexture);
  }
}

#endif
