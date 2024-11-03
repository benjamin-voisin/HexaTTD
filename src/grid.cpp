#include "grid.hpp"
#include "hex.hpp"

#include <math.h>
#include <assert.h>

Grid::Grid(Orientation orientation, Vector2 size, Vector2 origin, int q_min, int q_max, int r_min, int r_max) :
	q_min{q_min}, q_max{q_max}, r_min{r_min}, r_max{r_max}
	, tiles((r_max - r_min) * (q_max - q_min)) {
	this->layout = new Layout(orientation, size, origin);
}

Grid::~Grid() {
	delete layout;
}


void Grid::draw() {
	for (int q = q_min; q <= q_max; q++) {
		for (int r = r_min; r <= r_max; r++) {
			Hex(q, r).draw(*layout, RED);
		}
	}
	int n_classes = graph.get_max_class();
	for (long unsigned i=0; i<rails.size(); ++i) {
		int r_class = graph.get_class(i);
		rails[i].draw(*this->layout, ColorFromHSV(((float) r_class/ (float) n_classes)*360, 0.7f, 0.5f));
	}
}

void Grid::hightlight(Hex hex, Color c) {
	hex.draw( *layout, c);
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
    const Orientation& M = layout->orientation;
    Vector2 pt = Vector2 {(x - layout->origin.x) / layout->size.x,
                     (y - layout->origin.y) / layout->size.y};
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

void Grid::add_rail(Hex hex, int src_side, int dst_side, int width) {
	Tile* src_neighbor = tile_from_hex(hex.neighbor(src_side));
	Tile* tile = tile_from_hex(hex);
	Tile* dst_neighbor = tile_from_hex(hex.neighbor(dst_side));

	std::vector<int> src_edges = tile->get_rails(src_side);
	std::vector<int> dst_edges = tile->get_rails(dst_side);
		 	
	std::vector<int> edges = {};
	edges.insert(edges.end(), src_edges.begin(), src_edges.end());
	edges.insert(edges.end(), dst_edges.begin(), dst_edges.end());
	
	int track_id = graph.add(edges);
	tile->add_on_tile_track(track_id);
	src_neighbor->add_rail(Hex::opposite_direction(src_side), track_id);
	dst_neighbor->add_rail(Hex::opposite_direction(dst_side), track_id);
	
	assert(rails.size() == (long unsigned) track_id);
	rails.push_back(Rail(hex, src_side, dst_side, width));
}
