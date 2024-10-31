#include "grid.hpp"
#include "hex.hpp"

#include <math.h>

Grid::Grid(Orientation orientation, Vector2 size, Vector2 origin, int q_min, int q_max, int r_min, int r_max) :
	q_min{q_min}, q_max{q_max}, r_min{r_min}, r_max{r_max} {
	layout = new Layout(orientation, size, origin);
}

void Grid::draw() {
	for (int q = q_min; q <= q_max; q++) {
		for (int r = r_min; r <= r_max; r++) {
			Hex(q, r).draw(*layout, RED);
		}
	}
}

void Grid::hightlight(Hex hex) {
	hex.draw( *layout, GREEN);
}

Hex round(float q, float r, float s) {
    double rx = round(q);
    double ry = round(r);
    double rz = round(s);

    double x_diff = abs(rx - q);
    double y_diff = abs(ry - r);
    double z_diff = abs(rz - s);

    if (x_diff > y_diff and x_diff > z_diff) {
        rx = -ry-rz;
	} else {
		if (y_diff > z_diff) {
			ry = -rx-rz;
		} else {
			rz = -rx-ry;
		}
	}
    return Hex(rx, ry, rz);
}

Hex Grid::xy_to_hex(float x, float y) {
    const Orientation& M = layout->orientation;
    Vector2 pt = Vector2 {(x - layout->origin.x) / layout->size.x,
                     (y - layout->origin.y) / layout->size.y};
    double q = M.b0 * pt.x + M.b1 * pt.y;
    double r = M.b2 * pt.x + M.b3 * pt.y;

	// We have fractionnal q and r, we now need to convert them to the correct
	// values

    return round(q, r, -q -r);
}
