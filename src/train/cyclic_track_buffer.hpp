#pragma once

#include <stdio.h>
#include <stdlib.h>

struct prev_rail_s {
    int rail_id;
    int direction;
};

class Cyclic_buffer {
  protected:
    size_t _size;
    size_t _sizemax;
    size_t _index;
    int _incr;
    prev_rail_s *_buffer;

  private:
    size_t index_min();

  public:
    Cyclic_buffer(size_t size);
    ~Cyclic_buffer();

    void pp(FILE *f);

    size_t get_size();
    size_t get_max_size();

    void add_prev_rail(int rail_id, int direction);
    prev_rail_s del_last_prev_rail();
    prev_rail_s get_prev_rail(size_t n);

    void reverse();
};
