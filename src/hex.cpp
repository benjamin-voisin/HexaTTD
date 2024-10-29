#include <math.h>
#include <assert.h>

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


void Hex::draw(Grid grid) {
	// Calculate position center from coordinates
	Vector2 c = center(grid);
    DrawPoly( c, 6, _radius, 0, WHITE);
    DrawPolyLinesEx(c, 6, _radius, 0, 10, RED);
}
