#include "train.hpp"

#include "../raylib/src/raylib.h"
#include "graphics/display_train.hpp"

Train::Train(Rail *rail) :
	_position{rail} {
		_orientation = 0;
		_current_speed = 0;
		_max_speed = 100;
		_progression = 0.3f;
}

void Train::draw(Layout layout) {
	// First put the train in the center of the tile
	auto position = _position->get_position(layout, _progression);
	DrawTrain train = DrawTrain(position.position.to_Vector2(), {30, 70}, position.direction.angle());
	train.draw();
}
