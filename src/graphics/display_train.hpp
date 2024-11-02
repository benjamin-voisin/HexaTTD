#pragma once

#include "../../raylib/src/raylib.h"

class DrawTrain {
	private:
		Vector2 _position;
		Vector2 _size;
		float _angle;
	public:
		void draw();
		DrawTrain(Vector2 position, Vector2 size, float angle);
};
