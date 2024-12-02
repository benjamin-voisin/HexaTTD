#include <assert.h>
#include <math.h>
#include <vector>

#include "../raylib/src/raylib.h"
#include "hex.hpp"

Hex::Hex(int q, int r, int s) : _q{q}, _r{r}, _s{s} {
    assert(q + r + s == 0);
    coordinates[0] = _q;
    coordinates[1] = _r;
    coordinates[2] = _s;
};

Hex::Hex(int q, int r) : Hex(q, r, -q - r) {};

Hex::Hex(Vector v) : Hex(v.x, v.y) {};

int Hex::get_q() { return _q; }

int Hex::get_r() { return _r; }

int Hex::get_s() { return _s; }

bool operator==(Hex a, Hex b) {
    return a._q == b._q && a._r == b._r && a._s == b._s;
}

bool operator!=(Hex a, Hex b) { return !(a == b); }

Hex operator+(Hex a, Hex b) { return a.add(b); }

Hex operator-(Hex a, Hex b) { return a.subtract(b); }

Hex operator*(Hex a, int k) { return a.multiply(k); }

Vector Hex::to_Vector() { return Vector(_q, _r); }

Hex Hex::add(Hex a) { return Hex(to_Vector() + a.to_Vector()); }

Hex Hex::subtract(Hex a) { return Hex(_q - a._q, _r - a._r); }

Hex Hex::multiply(int k) { return Hex(_q * k, _r * k); }

int Hex::length() { return int((abs(_q) + abs(_r) + abs(_s)) / 2); }

int Hex::distance(Hex a) { return subtract(a).length(); }

bool Hex::is_neighbor(Hex other) { return distance(other) == 1; }

Vector hex_directions[6] = {Vector(1, 0),  Vector(0, 1),  Vector(-1, 1),
                            Vector(-1, 0), Vector(0, -1), Vector(1, -1)};

int Hex::opposite_direction(int direction) {
    return ((direction + 3) % 6 + 6) % 6;
}

Hex hex_direction(int direction) {
    direction = ((direction % 6) + 6) % 6;
    assert(0 <= direction && direction < 6);
    return Hex(hex_directions[direction]);
}

Hex Hex::neighbor(int direction) { return add(hex_direction(direction)); }
int Hex::direction() {
    Vector v = to_Vector();
    if (v == Vector(1, 0))
        return 0;
    if (v == Vector(0, 1))
        return 1;
    if (v == Vector(-1, 1))
        return 2;
    if (v == Vector(-1, 0))
        return 3;
    if (v == Vector(0, -1))
        return 4;
    if (v == Vector(1, -1))
        return 5;
    assert(false);
    return -1;
}

Vector2 raw_center(Layout layout, Vector v) {
    float x = (layout.orientation.f0 * v.x + layout.orientation.f1 * v.y) *
              layout.size.x;
    float y = (layout.orientation.f2 * v.x + layout.orientation.f3 * v.y) *
              layout.size.y;
    return Vector2{x + layout.origin.x, y + layout.origin.y};
}
Vector2 Hex::center_side(Layout layout, int direction) {
    return raw_center(layout, to_Vector() + hex_directions[direction] / 2);
}
Vector2 Hex::center(Layout layout) { return raw_center(layout, to_Vector()); }

float Hex::corner_angle(Layout layout, int corner) {
    return 2.0 * M_PI * (layout.orientation.start_angle + corner) / 6;
}

Vector2 corner_offset(Layout layout, int corner) {
    float angle = Hex::corner_angle(layout, corner);
    return Vector2{layout.size.x * cos(angle), layout.size.y * sin(angle)};
}

Vector2 Hex::corner(Layout layout, int corner) {
    Vector2 c = center(layout);
    Vector2 offset = corner_offset(layout, (corner + 6) % 6);
    return Vector2{c.x + offset.x, c.y + offset.y};
}

std::vector<Vector2> Hex::corners(Layout layout) {
    std::vector<Vector2> corners = {};
    for (int i = 0; i < 6; i++) {
        corners.push_back(corner(layout, i));
    }
    corners.push_back(corners[0]);
    return corners;
}

Base Hex::base_sides(Layout layout, int k) {
    return Base{corner_angle(layout, k)};
}

bool Hex::is_visible(Layout layout) {
    auto c = center(layout);
    if (c.x - layout.size.x > layout.screen_width) {
        return false;
    }
    if (c.x + layout.size.x < 0) {
        return false;
    }
    if (c.y - layout.size.y > layout.screen_height) {
        return false;
    }
    if (c.y + layout.size.y < 0) {
        return false;
    }
    return true;
}

void Hex::draw(Layout layout, Color color) {
    if (is_visible(layout)) {
        std::vector<Vector2> corners_list = corners(layout);
        DrawLineStrip(&corners_list[0], 7, color);
    }
}

/* #ifdef TEST */

#include <iostream>

HexTest::HexTest() {
	name = "Hex.cpp";
}

HexTest::~HexTest() { }

void HexTest::run_test() {
	std::cout << "HEXAGONES " << "\n";
}


/* #endif // TEST */
