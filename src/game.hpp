#pragma once

#include "grid.hpp"

#include <thread>

class Game {
  private:
    Grid _grid;
    std::thread _update_thread;
    std::thread _draw_thread;
    bool _running;
    std::string _name;

  public:
    void start();
    Game(int width, int height, std::string name);
    void wait();
};
