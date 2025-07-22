#include "display_station.hpp"

DrawStation::DrawStation(Vector position)
    : _position{position} {}

void DrawStation::draw() {
    DrawCircleV(_position.to_Vector2(), 30, BLACK); // Draw "position" point
    DrawCircleV(_position.to_Vector2(), 25, WHITE); // Draw "position" point

    /* DrawCircleV(position.to_Vector2(), 10, GREEN); // Draw start of the draw
     * point */
}
