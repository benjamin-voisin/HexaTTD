#pragma once

#include <math.h>

#include "layout.hpp"
#include "hex.hpp"

class Grid {
	private:
		int q_min;
		int q_max;
		int r_min;
		int r_max;

	public:
		Layout *layout;
		void draw();
		void hightlight(Hex hex);
		Grid(Orientation orientation, Vector2 size, Vector2 origin, int q_min, int q_max, int r_min, int r_max);
		Hex xy_to_hex(float x, float y);
};
