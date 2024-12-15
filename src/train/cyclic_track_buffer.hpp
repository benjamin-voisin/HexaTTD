#pragma once

#include <stdlib.h>
#include <stdio.h>

struct prev_rail_s {
	int rail_id;
	int direction;
};

class Cyclic_buffer {
    protected:
        std::size_t _size;
        std::size_t _sizemax;
        std::size_t _index;
        int _incr;
        prev_rail_s* _buffer;

    private:
        std::size_t index_min();

    public:
        Cyclic_buffer(std::size_t size);
        ~Cyclic_buffer();

        void pp(FILE* f);

        std::size_t get_size();
        std::size_t get_max_size();

        void add_prev_rail(int rail_id, int direction);
        prev_rail_s del_last_prev_rail();
        prev_rail_s get_prev_rail(std::size_t n);

        void reverse();
};