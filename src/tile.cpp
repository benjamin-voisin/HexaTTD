#include "tile.hpp"

#include <assert.h>

Tile::Tile() {};

std::vector<int> Tile::get_rails(int direction) {
    assert((0 <= direction) && (direction < 6));
    return this->neighbor_tracks[direction];
}

void Tile::add_rail(int direction, int id_rail) {
    assert((0 <= direction) && (direction < 6));
    this->neighbor_tracks[direction].push_back(id_rail);
}
void Tile::add_on_tile_track(int id_rail) {
    this->on_tile_tracks.push_back(id_rail);
}
void Tile::pp(FILE* f) {
    for (int dir=0; dir<6; ++dir) {
        fprintf(f, "dir_%d = ", dir);
        fprintf(f, "[");
        for (auto r=neighbor_tracks[dir].begin(); r<neighbor_tracks[dir].end(); ++r) {
            if (r > neighbor_tracks[dir].begin())
                fprintf(f, ", ");
            fprintf(f, "%d", *r);
        }
        fprintf(f, "]\n");
    }
    fprintf(f, "on_tile = ");
    fprintf(f, "[");
    for (auto r=on_tile_tracks.begin(); r<on_tile_tracks.end(); ++r) {
        if (r > on_tile_tracks.begin())
            fprintf(f, ", ");
        fprintf(f, "%d", *r);
    }
    fprintf(f, "]\n");
}