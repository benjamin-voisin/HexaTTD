#include <math.h>
#include <assert.h>
#include <vector>

#include "hex.hpp"
#include "../raylib/src/raylib.h"


Hex::Hex(int q, int r) : _q{q}, _r{r} {
	_s = -q -r;
	coordinates[0] = _q;
	coordinates[1] = _r;
	coordinates[2] = _s;
}

Hex::Hex(int q, int r, int s) : _q{q}, _r{r}, _s{s} {
	assert(q + r + s == 0);
	coordinates[0] = _q;
	coordinates[1] = _r;
	coordinates[2] = _s;
};

bool operator == (Hex a, Hex b) {
	return a._q == b._q && a._r == b._r && a._s == b._s;
}

bool operator != (Hex a, Hex b) {
	return !(a == b);
}

Hex operator + (Hex a, Hex b) {
	return a.add(b);
}

Hex operator - (Hex a, Hex b) {
	return a.subtract(b);
}

Hex operator * (Hex a, int k) {
	return a.multiply(k);
}

Hex Hex::add(Hex a) {
    return Hex(_q + a._q, _r + a._r);
}

Hex Hex::subtract(Hex a) {
    return Hex(_q - a._q, _r - a._r);
}

Hex Hex::multiply(int k) {
    return Hex(_q * k, _r * k);
}

int Hex::length() {
	return int((abs(_q) + abs(_r) + abs(_s)) / 2);
}

int Hex::distance(Hex a) {
	return subtract(a).length();
}

Hex hex_directions[6] = {
    Hex(1, 0), Hex(1, -1), Hex(0, -1),
    Hex(-1, 0), Hex(-1, 1), Hex(0, 1)
};

Hex hex_direction(int direction) {
	assert (0 <= direction && direction < 6);
    return hex_directions[direction];
}

Hex Hex::neighbor(int direction) {
	return add(hex_direction(direction));
}

Vector2 Hex::center(Layout layout) {
    float x = (layout.orientation.f0 * _q + layout.orientation.f1 * _r) * layout.size.x;
    float y = (layout.orientation.f2 * _q + layout.orientation.f3 * _r) * layout.size.y;
    return Vector2 {x + layout.origin.x, y + layout.origin.y};
}

float Hex::corner_angle(Layout layout, int corner) {
	return 2.0 * M_PI *
             (layout.orientation.start_angle + corner) / 6;
}

Vector2 corner_offset(Layout layout, int corner) {
    float angle = Hex::corner_angle(layout, corner);
    return Vector2 {layout.size.x * cos(angle), layout.size.y * sin(angle) };
}

std::vector<Vector2> Hex::corners(Layout layout) {
	std::vector<Vector2> corners = {};
    Vector2 c = center(layout);
    for (int i = 0; i < 6; i++) {
        Vector2 offset = corner_offset(layout, i);
        corners.push_back(Vector2 {c.x + offset.x, c.y + offset.y});
    }
	corners.push_back(corners[0]);
    return corners;
}

Base Hex::base_sides(Layout layout, int k) {
	return Base{corner_angle(layout, k)};
}

void Hex::draw(Layout layout, Color color) {
	std::vector<Vector2> corners_list = corners(layout);
	/* Vector2 fan[8]; */
	/* fan[0] = center(layout); */
	/* for (int i = 1; i < 7; i++) { */
	/* 	fan[7 - i] = corners_list[i]; */
	/* } */
	/* fan[7] = corners_list[0]; */
	/* DrawTriangleFan(fan, 8, WHITE); */
	DrawLineStrip(&corners_list[0], 7, color);
}
