#include "tile.hpp"

#include <assert.h>

Tile::Tile() {};

std::vector<int> Tile::get_rails(int direction) {
    assert((0 <= direction) && (direction < 6));
    return this->rails[direction];
}

void Tile::add_rail(int direction, int id_rail) {
    assert((0 <= direction) && (direction < 6));
    this->rails[direction].push_back(id_rail);
}

void Tile::pp(FILE* f) {
    for (int dir=0; dir<6; ++dir) {
        fprintf(f, "dir_%d = ", dir);
        fprintf(f, "[");
        for (auto r=rails[dir].begin(); r<rails[dir].end(); ++r) {
            if (r > rails[dir].begin())
                fprintf(f, ", ");
            fprintf(f, "%d", *r);
        }
        fprintf(f, "]\n");
    }
}