#pragma once

#include "../../raylib/src/raylib.h"
#include "../vector.hpp"

class DrawTrain {
	private:
		Vector _position;
		Vector _size;
		Vector _direction;
	public:
		void draw();
		DrawTrain(Vector position, Vector size, Vector direction);
};
