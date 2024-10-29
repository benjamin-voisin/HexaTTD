#include "grid.hpp"
#include "hex.hpp"

Grid::Grid(Orientation orientation, Vector2 size, Vector2 origin, int q_min, int q_max, int r_min, int r_max) :
	q_min{q_min}, q_max{q_max}, r_min{r_min}, r_max{r_max} {
	layout = new Layout(orientation, size, origin);
}

void Grid::draw() {
	for (int q = q_min; q <= q_max; q++) {
		for (int r = r_min; r <= r_max; r++) {
			Hex(q, r).draw(*layout);
		}
	}
}
