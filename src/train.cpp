#include "train.hpp"

#include "../raylib/src/raylib.h"

Train::Train(Rail *rail) :
	_position{rail} {
		_orientation = 0;
		_current_speed = 0;
		_max_speed = 100;
		_progression = 0.f;
}

void Train::draw(Layout layout) {
	// First put the train in the center of the tile
	auto center = _position->get_hex().center(layout);
	DrawRectangle(center.x, center.y, 30, 70, RED);
}
