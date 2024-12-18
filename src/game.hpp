#pragma once

#include "grid.hpp"
#include "gui/toggle.hpp"

#include <thread>

struct Game {
  private:
    Grid _grid;
    std::thread _update_thread;
    std::thread _draw_thread;
    bool _running;
    std::string _name;
    GuiToggleElement _debug_toggle;

  public:
    void update();
    void draw();
    void start();
    Game(int width, int height, std::string name);
    void wait();
};
