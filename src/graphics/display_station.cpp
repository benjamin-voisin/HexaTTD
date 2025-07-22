#include "display_station.hpp"

DrawStation::DrawStation(Vector position)
    : _position{position} {}

void DrawStation::draw(Layout *layout) {
    DrawCircleV(_position.to_Vector2(), 3 * layout->size.x / 10, BLACK); // Draw "position" point
    DrawCircleV(_position.to_Vector2(), 2.5 * layout->size.x / 10, WHITE); // Draw "position" point

    /* DrawCircleV(position.to_Vector2(), 10, GREEN); // Draw start of the draw
     * point */
}
