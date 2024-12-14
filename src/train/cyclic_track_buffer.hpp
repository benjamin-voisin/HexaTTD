#pragma once

#include <stdlib.h>

struct prev_rail {
	int rail_id;
	int direction;
};

class Cyclic_buffer {
    protected:
        std::size_t _size;
        std::size_t _index;
        prev_rail* _buffer;

    public:
        Cyclic_buffer(std::size_t size);
        ~Cyclic_buffer();

        void add_prev_rail(int rail_id, int direction);
        struct prev_rail get_prev_rail(int n);
};