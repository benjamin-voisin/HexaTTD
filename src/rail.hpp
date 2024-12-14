#pragma once

#include "hex.hpp"
#include "layout.hpp"

#include <set>
#include <stdio.h>

struct train_pos {
    Vector position;
    Vector direction;
};

class Rail {
  private:
    Hex _hex;
    int _src_neighbor; // Integer between 0 and 11 included, the entry point
    int _dst_neighbor; // Integer between 0 and 11 included, exit point
    int _width;        // The spacing between two rails
    std::set<int> _on_track_train;

  public:
    void pp(FILE *f);
    bool deleted;
    Hex get_hex();
    Rail(Hex tile, int src_side, int dst_side, int width);

    int get_src_neighbor();
    int get_dst_neighbor();
    std::set<int> get_on_track_train();
    void get_on_track_train(int train);
    void del_on_track_train(int train);



    void draw(Layout layout, Color c, int phase);
    void draw(Layout layout, Color c);
    train_pos get_position(Layout layout, float progression);
    bool is_on_track(Layout layout, Vector v);
    static int number_phases();
    
};
