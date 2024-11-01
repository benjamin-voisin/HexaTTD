#pragma once

#include <vector>
#include <stdio.h>

class Tile {
    protected:
        std::vector<int> rails[6];

    public:
        Tile();
        std::vector<int> get_rails(int direction);
        void add_rail(int direction, int id_rail);
        void pp(FILE* f);
};