#pragma once

#include "../vector.hpp"
#include "raylib.h"

class DrawTrain {
  private:
    Vector _position;
    Vector _size;
    Vector _direction;

  public:
    void draw();
    DrawTrain(Vector position, Vector size, Vector direction);
};
