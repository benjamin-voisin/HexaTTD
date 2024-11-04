#pragma once

#include "hex.hpp"
#include "layout.hpp"

struct train_pos {
	Vector position;
	Vector direction;
};

class Rail {
	private:
		Hex _hex;
		int _src_neighbor;	// Integer between 0 and 11 included, the entry point
		int _dst_neighbor;		// Integer between 0 and 11 included, exit point
		int _width;		// The spacing between two rails
	public:
		Hex get_hex();
		Rail(Hex tile, int src_side, int dst_side, int width);

		int get_src_neighbor();
		int get_dst_neighbor();
		
		void draw(Layout layout, Color c);
		train_pos get_position(Layout layout, float progression);
};
