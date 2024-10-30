#include "rail.hpp"
#include "../raylib/src/raylib.h"

void Rail::draw(Layout layout) {
	Vector2 center = _tile.center(layout);
	std::vector<Vector2> corners = _tile.corners(layout);

	Vector2 c1 = corners[_entry_point];
	Vector2 c2 = corners[_entry_point  + 1];
	Vector2 c3 = corners[_exit_point];
	Vector2 c4 = corners[_exit_point + 1];

	Vector2 p1[3];
	p1[1] = center;
	p1[0] = GetSplinePointLinear(c1, c2, 0.25);
	p1[2] = GetSplinePointLinear(c3, c4, 0.75);
	Vector2 p2[3];
	p2[1] = center;
	p2[0] = GetSplinePointLinear(c1, c2, 0.75);
	p2[2] = GetSplinePointLinear(c3, c4, 0.25);

	DrawSplineBezierQuadratic(p1, 3, _width, BLACK);
	DrawSplineBezierQuadratic(p2, 3, _width, BLACK);
}
