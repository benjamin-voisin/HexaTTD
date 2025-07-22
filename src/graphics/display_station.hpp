#pragma once

#include "geometry/vector.hpp"
#include "geometry/layout.hpp"

class DrawStation {
  private:
    Vector _position;

  public:
    void draw(Layout *layout);
    DrawStation(Vector position);
};
