#include "display_train.hpp"

DrawTrain::DrawTrain(Vector2 position, Vector2 size, float angle)
	: _position{position}, _size{size}, _angle{angle} {}

void DrawTrain::draw() {
	DrawRectanglePro({_position.x, _position.y, 30, 70}, {0,0}, _angle, RED);
}
