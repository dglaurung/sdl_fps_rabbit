#include "FpsCounter.h"
#include <SDL/SDL.h>

FpsCounter::FpsCounter(int time_window) : _time_window(time_window), _cnt(0) {
}

std::shared_ptr<FpsCounter> FpsCounter::create(int time_window) {
  return std::make_shared<FpsCounter>(time_window);
}

void FpsCounter::_trim_queue() {
  while (!q.empty() && q[0] < SDL_GetTicks() - _time_window) {
    q.pop_front();
    --_cnt;
  }
}

void FpsCounter::frame_rendered() {
  q.push_back(SDL_GetTicks());
  _cnt += 1;
  _trim_queue();
}

float FpsCounter::get_current_fps() {
  _trim_queue();
  return (float(_cnt) / _time_window) * 1000.0f;
}