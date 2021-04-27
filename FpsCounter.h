#ifndef CALYPSO_GUI_CPP_SRC_CALYPSO_UI_COMMONS_FPSCOUNTER_H_
#define CALYPSO_GUI_CPP_SRC_CALYPSO_UI_COMMONS_FPSCOUNTER_H_

#include <deque>
#include <memory>

class FpsCounter {
 public:
  FpsCounter(int time_window);

  static std::shared_ptr<FpsCounter> create(int time_window);

  void frame_rendered();

  float get_current_fps();

 private:
  void _trim_queue();

  std::deque<long> q;
  int _time_window;
  int _cnt;
};


#endif //CALYPSO_GUI_CPP_SRC_CALYPSO_UI_COMMONS_FPSCOUNTER_H_
