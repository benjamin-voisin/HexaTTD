#include "train.hpp"
#include <algorithm>
#include <assert.h>
#include <stdio.h>

#include "../raylib/src/raylib.h"
#include "graphics/display_train.hpp"
#define MIN(a, b) ((a)<(b)? (a) : (b))

Train::Train(int track_id) {
	_rail_id = track_id;
	_orientation = 0;
	_current_speed = 0;
	_max_speed = 100;
	_progression = 0.f;
	_direction = 1;
}

void Train::draw(Layout layout, std::vector<Rail> rails) {
	// First put the train in the center of the tile
	auto position = rails[_rail_id].get_position(layout, _progression);
	DrawTrain train = DrawTrain(position.position, Vector(30, 70) * layout.size.x / 100, position.direction);
	train.draw();
}

void Train::next_rail(Graph graph, std::vector<Rail> rails) {
	std::vector<int> neighbor;
	int new_src_dir;
	if (_direction == 1) {
		// Dans ce cas,  le tain à traversé le dernier rail de la source vers la destination
		neighbor = graph.get_dst_neighbor(_rail_id);
		new_src_dir = Hex::opposite_direction(rails[_rail_id].get_dst_neighbor());			
	} else {
		assert(_direction == -1);
		// Dans ce cas, le train à traversé le dernier rail de la destination vers la source
		neighbor = graph.get_src_neighbor(_rail_id);
		new_src_dir = Hex::opposite_direction(rails[_rail_id].get_src_neighbor());
	}

	if (neighbor.size() > 0) {
		_rail_id = neighbor[std::rand() % neighbor.size()];
		printf("next rail = %d\n", _rail_id);
		if (new_src_dir == rails[_rail_id].get_src_neighbor()) {
			// Dans ce cas, on vas maintenant aller de la source à la destination
			printf("dir=destination\n");
			_progression = 0.f;
			_direction = 1;
		} else {
			assert(new_src_dir == rails[_rail_id].get_dst_neighbor());
			// Dans ce cas, on vas alors aller de la destination vers la source
			printf("dir=source\n");
			_progression = 1.f;
			_direction = -1;
		}
	} else {
		_direction *= -1;
	}
}

void Train::update(Graph graph, std::vector<Rail> rails) {
	_progression += 0.01 * _direction;
	if (_progression > 1.f || _progression < 0.f) {
		next_rail(graph, rails);
	}
}

ItineraryTrain::ItineraryTrain(std::vector<int> path) :
	Train(path[0]), _path{path}, _position{0}
	{ assert(path.size() > 0); }

void ItineraryTrain::next_rail(__attribute__((unused)) Graph graph, std::vector<Rail> rails) {
	if ((long unsigned) _position < _path.size() - 1) {
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
	_rail_id = _path[_position];
}