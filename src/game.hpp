#pragma once

#include "grid.hpp"
#include "gui/gui.hpp"

#include <semaphore>
#include <thread>

struct Game {
  private:
    Grid _grid;
    std::thread _update_thread;
    std::thread _draw_thread;
    bool _running;
    std::string _name;
    Gui _gui;
    std::binary_semaphore _start_sema;
    bool _is_fast;
    void set_fullscreen();

  public:
    void update();
    void draw();
    void start();
    Game(int width, int height, std::string name);
    void wait();
};
