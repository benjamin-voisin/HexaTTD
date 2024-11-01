#pragma once

#include <vector>
#include <stdio.h>

class Tile {
    protected:
        std::vector<int> neighbor_tracks[6];
        std::vector<int> on_tile_tracks;

    public:
        Tile();
        std::vector<int> get_rails(int direction);
        void add_rail(int direction, int id_rail);
        void add_on_tile_track(int id_rail);
        void pp(FILE* f);
};