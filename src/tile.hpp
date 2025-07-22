#pragma once

#include <set>
#include <stdio.h>
#include "geometry/hex.hpp"

class Tile {
  protected:
    std::set<int> neighbor_tracks[6];
    std::set<int> on_tile_tracks;
    Hex hex;

  public:
    Tile(Hex hex);
    std::set<int> get_rails(int direction);
    void add_rail(int direction, int id_rail);
    void del_rail(int direction, int id_rail);
    void add_on_tile_track(int id_rail);
    void del_on_tile_track(int id_rail);
    std::set<int> get_rails_on_tile();
    void draw(Layout *layout, Color c);
    void pp(FILE *f);
};
