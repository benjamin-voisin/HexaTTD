#pragma once

#include "../geometry/vector.hpp"
#include "../geometry/hex.hpp"

class DrawTile {
  private:
    Hex _hex;
    Layout *_layout;
    Color _color;
    
  public:
    void draw();
    DrawTile(Hex hex, Layout *layout, Color color);
};
