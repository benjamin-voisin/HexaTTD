#include "display_tile.hpp"

DrawTile::DrawTile(Hex hex, Layout *layout, Color color)
    : _hex{hex}, _layout{layout}, _color{color} {}

void DrawTile::draw() {
    std::vector<Vector2> corners_list = _hex.corners(_layout);
    DrawLineStrip(&corners_list[0], 7, _color);
}
