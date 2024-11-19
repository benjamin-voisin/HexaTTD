#pragma once

#include <math.h>

#include "raylib.h"

class Orientation {
	public:
		const float f0, f1, f2, f3;
		const float b0, b1, b2, b3;
		const float start_angle; // in multiples of 60°
		Orientation(float f0_, float f1_, float f2_, float f3_,
				float b0_, float b1_, float b2_, float b3_,
				float start_angle_)
			: f0(f0_), f1(f1_), f2(f2_), f3(f3_),
			b0(b0_), b1(b1_), b2(b2_), b3(b3_),
			start_angle(start_angle_) {}
};

const Orientation layout_pointy
  = Orientation(sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0,
                sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0,
                0.5);
const Orientation layout_flat
  = Orientation(3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0),
                2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0,
                0.0);

class Layout {
	public:
		Orientation orientation;
		Vector2 size;
		Vector2 origin;
		Layout(Orientation orientation, Vector2 size, Vector2 origin) :
			orientation{orientation}, size{size}, origin{origin} {}
};
