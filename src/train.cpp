#include "train.hpp"
#include <algorithm>
#include <assert.h>

#include "../raylib/src/raylib.h"
#include "graphics/display_train.hpp"
#define MIN(a, b) ((a)<(b)? (a) : (b))

Train::Train(std::vector<int> path) :
	_path{path} {
		assert(path.size() > 0);
		_position = path[0];
		_orientation = 0;
		_direction = 1;
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
	_progression += 0.01 * _direction;
	if (_progression > 1.f || _progression < 0.f) {
		if ((long unsigned int) _position < _path.size() - 1) {
			int new_src_dir;
			if (_direction == 1) {
				// Dans ce cas,  le tain à traversé le dernier rail de la source vers la destination
				new_src_dir = Hex::opposite_direction(rails[_path[_position]].get_dst_neighbor());			
			} else {
				assert(_direction == -1);
				// Dans ce cas, le train à traversé le dernier rail de la destination vers la source
				new_src_dir = Hex::opposite_direction(rails[_path[_position]].get_src_neighbor());
			}
			if (new_src_dir == rails[_path[_position + 1]].get_src_neighbor()) {
				// Dans ce cas, on vas maintenant aller de la source à la destination
				_progression = 0.f;
				_direction = 1;
			} else {
				assert(new_src_dir == rails[_path[_position + 1]].get_dst_neighbor());
				// Dans ce cas, on vas alors aller de la destination vers la source
				_progression = 1.f;
				_direction = -1;
			}
			_position++;
		} else {
			// We flip the path
			_direction *= -1;
			_position = 0;
			std::reverse(_path.begin(), _path.end());
		}
	}
}
