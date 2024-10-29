
#include "rail.hpp"
#include "../raylib/src/raylib.h"

void Rail::draw(Layout layout) {
	Vector2 center = _tile.center(layout);
	std::vector<Vector2> corners = _tile.corners(layout);

	Vector2 p1[3];
	p1[1] = center;
	p1[0] = GetSplinePointLinear(corners[1], corners[2], 0.25);
	p1[2] = GetSplinePointLinear(corners[5], corners[6], 0.75);
	Vector2 p2[3];
	p2[1] = center;
	p2[0] = GetSplinePointLinear(corners[1], corners[2], 0.75);
	p2[2] = GetSplinePointLinear(corners[5], corners[6], 0.25);

	DrawSplineBezierQuadratic(p1, 3, _spacing / 2, BLACK);
	DrawSplineBezierQuadratic(p2, 3, _spacing / 2, BLACK);
}
