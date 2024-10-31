#pragma once

#include "hex.hpp"
#include "layout.hpp"

class Rail {
	private:
		Hex _tile;
		int _src_neighbor;	// Integer between 0 and 11 included, the entry point
		int _dst_neighbor;		// Integer between 0 and 11 included, exit point
		int _width;		// The spacing between two rails
	public:
		Rail(Hex tile, int src_side, int dst_side, int width);
		
		void draw(Layout layout);
};
