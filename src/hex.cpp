#include <math.h>
#include <assert.h>
#include <vector>

#include "hex.hpp"
#include "../raylib/src/raylib.h"


Hex::Hex(int q, int r, int radius) : _q{q}, _r{r}, _radius{radius} {
	_s = -q -r;
	coordinates[0] = _q;
	coordinates[1] = _r;
	coordinates[2] = _s;
}

Hex::Hex(int q, int r, int s, int radius) : _q{q}, _r{r}, _s{s}, _radius{radius} {
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
    return Hex(_q + a._q, _r + a._r, _s + a._s, _radius);
}

Hex Hex::subtract(Hex a) {
    return Hex(_q - a._q, _r - a._r, _s - a._s, _radius);
}

Hex Hex::multiply(int k) {
    return Hex(_q * k, _r * k, _s * k, _radius);
}

int Hex::length() {
	return int((abs(_q) + abs(_r) + abs(_s)) / 2);
}

int Hex::distance(Hex a) {
	return subtract(a).length();
}

Hex hex_directions[6] = {
    Hex(1, 0, -1, 100), Hex(1, -1, 0, 100), Hex(0, -1, 1, 100),
    Hex(-1, 0, 100), Hex(-1, 1, 0, 100), Hex(0, 1, -1, 100)
};

Hex hex_direction(int direction) {
	assert (0 <= direction && direction < 6);
    return hex_directions[direction];
}

Hex Hex::neighbor(int direction) {
	return add(hex_direction(direction));
}

Vector2 Hex::center(Grid grid) {
    float x = (grid.orientation.f0 * _q + grid.orientation.f1 * _r) * grid.size.x;
    float y = (grid.orientation.f2 * _q + grid.orientation.f3 * _r) * grid.size.y;
    return Vector2 {x + grid.origin.x, y + grid.origin.y};
}

Vector2 corner_offset(Grid grid, int corner) {
    float angle = 2.0 * M_PI *
             (grid.orientation.start_angle + corner) / 6;
    return Vector2 {grid.size.x * cos(angle), grid.size.y * sin(angle) };
}

std::vector<Vector2> Hex::corners(Grid grid) {
	std::vector<Vector2> corners = {};
    Vector2 c = center(grid);
    for (int i = 0; i < 6; i++) {
        Vector2 offset = corner_offset(grid, i);
        corners.push_back(Vector2 {c.x + offset.x, c.y + offset.y});
    }
	corners.push_back(corners[0]);
    return corners;
}


void Hex::draw(Grid grid) {
	std::vector<Vector2> corners_list = corners(grid);
	Vector2 fan[8];
	fan[0] = center(grid);
	for (int i = 1; i < 7; i++) {
		fan[7 - i] = corners_list[i];
	}
	fan[7] = corners_list[0];
	DrawTriangleFan(fan, 8, WHITE);
	DrawLineStrip(&corners_list[0], 7, RED);

}
