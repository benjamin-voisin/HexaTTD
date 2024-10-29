#pragma once

#include <vector>

#include "../raylib/src/raylib.h"
#include "layout.hpp"

class Hex {
	private:
		// We store the coordinates of the hex in evry possible way, we will see
		// what fits the most our needs
		int _q;
		int _r;
		int _s;
		int coordinates[3];

		Hex add(Hex a);
		Hex subtract(Hex a);
		Hex multiply(int k);


	public:
		void draw(Layout layout);
		Hex(int q, int r, int s);
		// As q + r + s = 0, we can build a constructor that computes s
		Hex(int q, int r);
		friend bool operator == (Hex a, Hex b);
		friend bool operator != (Hex a, Hex b);
		friend Hex operator + (Hex a, Hex b);
		friend Hex operator - (Hex a, Hex b);
		friend Hex operator * (Hex a, int k);

		int length();
		int distance(Hex a);
		Vector2 center(Layout layout);
		std::vector<Vector2> corners(Layout layout);

		Hex neighbor(int direction);

};


Hex hex_direction(int direction);
