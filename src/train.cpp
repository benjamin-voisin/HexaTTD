#include "train.hpp"

#include "../raylib/src/raylib.h"
#include "graphics/display_train.hpp"
#define MIN(a, b) ((a)<(b)? (a) : (b))

Train::Train(Rail *rail) :
	_position{rail} {
		_orientation = 0;
		_current_speed = 0;
		_max_speed = 100;
		_progression = 0.f;
}

void Train::draw(Layout layout) {
	// First put the train in the center of the tile
	auto position = _position->get_position(layout, _progression);
	DrawTrain train = DrawTrain(position.position, Vector(30, 70) * layout.size.x / 100, position.direction);
	train.draw();
}

void Train::update() {
	_progression += 0.01;
	if (_progression > 1.f) {
		_progression = 0.f;
	}
}
