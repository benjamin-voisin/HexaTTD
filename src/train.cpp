#include "train.hpp"
#include <assert.h>

#include "../raylib/src/raylib.h"
#include "graphics/display_train.hpp"
#define MIN(a, b) ((a)<(b)? (a) : (b))

Train::Train(std::vector<int> path) :
	_path{path} {
		assert(path.size() > 0);
		_position = path[0];
		_orientation = 0;
		_current_speed = 0;
		_max_speed = 100;
		_progression = 0.f;
}

void Train::draw(Layout layout, std::vector<Rail> rails) {
	// First put the train in the center of the tile
	auto position = rails[_path[_position]].get_position(layout, _progression);
	DrawTrain train = DrawTrain(position.position, Vector(30, 70) * layout.size.x / 100, position.direction);
	train.draw();
}

void Train::update(std::vector<Rail> rails) {
	_progression += 0.01;
	if (_progression > 1.f) {
		_progression = 0.f;
		if (_position < _path.size() - 1) {
			_position++;
		} else {
			_position = 0;
		}
	}
}
