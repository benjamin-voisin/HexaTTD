#include "display_train.hpp"

DrawTrain::DrawTrain(Vector position, Vector size, Vector direction)
	: _position{position}, _size{size}, _direction{direction.normalise()} {}

void DrawTrain::draw() {
	Vector position  = _position - (_direction.orthogonal() * _size.x / 2) + (_direction * _size.y / 2);
	DrawRectanglePro({position.x, position.y, _size.x, _size.y}, {0,0}, _direction.angle(), RED);
	/* DrawCircleV(_position.to_Vector2(), 10, BLUE); // Draw "position" point */
	/* DrawCircleV(position.to_Vector2(), 10, GREEN); // Draw start of the draw point */ 
}
