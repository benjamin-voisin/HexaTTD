#include "grid.hpp"
#include "hex.hpp"

#include <math.h>
#include <assert.h>

Grid::Grid(Orientation orientation, Vector2 size, Vector2 origin, int q_min, int q_max, int r_min, int r_max) :
	q_min{q_min}, q_max{q_max}, r_min{r_min}, r_max{r_max}
	, tiles((r_max - r_min) * (q_max - q_min)), layout{orientation, size, origin, GetScreenWidth(), GetScreenHeight()} {
}

Grid::~Grid() {
	for (unsigned long i = 0; i < trains.size(); i++) {
		delete trains[i];
	}
}


void Grid::draw() {
	for (int q = q_min; q <= q_max; q++) {
		for (int r = r_min; r <= r_max; r++) {
			Hex(q, r).draw(layout, BLACK);
		}
	}
	int n_classes = graph.get_max_class();
	for (int phase = 0; phase < Rail::number_phases(); phase++) {
		for (long unsigned i=0; i<rails.size(); ++i) {
			if (!rails[i].deleted) {
				int r_class = graph.get_class(i);
				rails[i].draw(this->layout, ColorFromHSV(((float) r_class/ (float) n_classes)*360, 0.7f, 0.5f), phase);
			}
		}
	}
	for (long unsigned i=0; i < stations.size(); i++) {
		stations[i].draw(layout, rails);
	}
	for (long unsigned i=0; i<trains.size(); i++) {
		trains[i]->draw(layout, rails);
	}
}

void Grid::hightlight(Hex hex, Color c) {
	hex.draw( layout, c);
}

Hex round(float q, float r, float s) {
    double rx = round(q);
    double ry = round(r);
    double rz = round(s);

    double x_diff = abs(rx - q);
    double y_diff = abs(ry - r);
    double z_diff = abs(rz - s);

    if (x_diff > y_diff and x_diff > z_diff) {
        rx = -ry-rz;
	} else {
		if (y_diff > z_diff) {
			ry = -rx-rz;
		} else {
			rz = -rx-ry;
		}
	}
    return Hex(rx, ry, rz);
}

Hex Grid::xy_to_hex(float x, float y) {
    const Orientation& M = layout.orientation;
    Vector2 pt = Vector2 {(x - layout.origin.x) / layout.size.x,
                     (y - layout.origin.y) / layout.size.y};
    double q = M.b0 * pt.x + M.b1 * pt.y;
    double r = M.b2 * pt.x + M.b3 * pt.y;

	// We have fractionnal q and r, we now need to convert them to the correct
	// values

    return round(q, r, -q -r);
}

Tile* Grid::tile_from_hex(Hex hex) {
	assert((q_min <= hex.get_q()) && (hex.get_q() <= q_max));
	assert((r_min <= hex.get_r()) && (hex.get_r() <= r_max));
	return &tiles[(hex.get_q() - q_min) * (q_max - q_min) + (hex.get_r() - r_min)];
}

void Grid::del_rail(int track_id) {
	assert((0 <= track_id) && ((long unsigned) track_id < rails.size()));
	Rail r = rails[track_id];

	Hex hex = r.get_hex();
	Tile* src_neighbor = tile_from_hex(hex.neighbor(r.get_src_neighbor()));
	Tile* tile = tile_from_hex(hex);
	Tile* dst_neighbor = tile_from_hex(hex.neighbor(r.get_dst_neighbor()));

	tile->del_on_tile_track(track_id);
	src_neighbor->del_rail(Hex::opposite_direction(r.get_src_neighbor()), track_id);
	dst_neighbor->del_rail(Hex::opposite_direction(r.get_dst_neighbor()), track_id);

	rails[track_id].deleted = true;

	graph.delete_rail(track_id);
}

void Grid::add_rail(Hex hex, int src_side, int dst_side, int width) {
	rails.push_back(Rail(hex, src_side, dst_side, width));
	Rail r = rails[rails.size()-1];
#ifndef NDEBUG
	printf("New rail %ld: src=%d dst=%d\n", rails.size()-1, r.get_src_neighbor(), r.get_dst_neighbor());
#endif // !NDEBUG
	Tile* src_neighbor = tile_from_hex(hex.neighbor(r.get_src_neighbor()));
	Tile* tile = tile_from_hex(hex);
	Tile* dst_neighbor = tile_from_hex(hex.neighbor(r.get_dst_neighbor()));

	std::set<int> src_edges = tile->get_rails(r.get_src_neighbor());
	std::set<int> dst_edges = tile->get_rails(r.get_dst_neighbor());
		 	
	int track_id = graph.add(rails[rails.size()-1], rails, src_edges, dst_edges);
	tile->add_on_tile_track(track_id);
	src_neighbor->add_rail(Hex::opposite_direction(r.get_src_neighbor()), track_id);
	dst_neighbor->add_rail(Hex::opposite_direction(r.get_dst_neighbor()), track_id);
	
	assert(rails.size()-1 == (long unsigned) track_id);
}

void Grid::add_train(Train* train) {
	trains.push_back(train);
}

Rail Grid::get_rail(int track_id) {
	return rails[track_id];
}

void Grid::add_station(int rail_id, std::string name) {
	stations.push_back(Station(rail_id, name));
}


void Grid::update() {
	layout.screen_width = GetScreenWidth();
	layout.screen_height = GetScreenHeight();
	for (long unsigned i=0; i<trains.size(); i++) {
		trains[i]->update(graph, rails);
	}
}
