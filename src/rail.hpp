#pragma once

#include "hex.hpp"
#include "layout.hpp"

class Rail {
	private:
		Hex _tile;
		int _entry_point;	// Integer between 0 and 11 included, the entry point
		int _exit_point;		// Integer between 0 and 11 included, exit point
		int _width;		// The spacing between two rails
	public:
		Rail(Hex tile, int entry_point, int exit_point, int width) :
			_tile{tile}, _entry_point{entry_point}, _exit_point{exit_point}, _width{width} {};

		void draw(Layout layout);
};
