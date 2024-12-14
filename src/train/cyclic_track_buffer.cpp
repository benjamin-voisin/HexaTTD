#include "cyclic_track_buffer.hpp"

Cyclic_buffer::Cyclic_buffer(std::size_t size) {
    _size = size;
    _index = 0;
    _buffer = (struct prev_rail *)calloc(size, sizeof(struct prev_rail));
    
}

Cyclic_buffer::~Cyclic_buffer() {
    free(_buffer);
}


void Cyclic_buffer::add_prev_rail(int rail_id, int direction) {
    _index = (_index + 1) % _size;
    _buffer[_index].rail_id = rail_id;
    _buffer[_index].direction = direction;
}

prev_rail Cyclic_buffer::get_prev_rail(int n) {
    if ((std::size_t) n > _index) {
        return _buffer[_index + _size - n];
    } else {
        return _buffer[_index - n];
    }
}
