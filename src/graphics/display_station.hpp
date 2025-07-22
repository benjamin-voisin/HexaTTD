#pragma once

#include "../geometry/vector.hpp"

class DrawStation {
  private:
    Vector _position;

  public:
    void draw();
    DrawStation(Vector position);
};
