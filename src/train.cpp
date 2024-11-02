#include "train.hpp"

#include "../raylib/src/raylib.h"
#include "graphics/display_train.hpp"

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
	DrawTrain train = DrawTrain(center, {30, 70}, 0);
	train.draw();
}
