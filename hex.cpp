#include "hex.hpp"
#include "./raylib/src/raylib.h"

hex::hex(int q, int r, int s) : _q{q}, _r{r}, _s{s} {};

void hex::draw() {
    DrawPolyLinesEx((Vector2){ 1000/4.0f*3, 330 }, 6, 85, 0, 6, BEIGE);
}
