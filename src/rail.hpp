#pragma once

#include "hex.hpp"
#include "layout.hpp"

class Rail {
	private:
		Hex _tile;
		int _entry_point;	// Integer between 0 and 11 included, the entry point
		int _exit_point;		// Integer between 0 and 11 included, exit point
		int _spacing;		// The spacing between two rails
	public:
		Rail(Hex tile, int entry_point, int exit_point, int spacing) :
			_tile{tile}, _entry_point{entry_point}, _exit_point{exit_point}, _spacing{spacing} {};

		void draw(Layout layout);
};
