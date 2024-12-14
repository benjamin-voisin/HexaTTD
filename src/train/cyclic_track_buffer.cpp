#include "cyclic_track_buffer.hpp"

int mod(int a, int b) {
    return ((a % b) + b) % b;
}

Cyclic_buffer::Cyclic_buffer(std::size_t size) {
    _size = size;
    _index = 0;
    _incr = 1;
    _buffer = (struct prev_rail *)calloc(size, sizeof(struct prev_rail));
    
}

Cyclic_buffer::~Cyclic_buffer() {
    free(_buffer);
}

void Cyclic_buffer::add_prev_rail(int rail_id, int direction) {
    _index = (_index + _incr) % _size;
    _buffer[_index].rail_id = rail_id;
    _buffer[_index].direction = direction;
}

prev_rail Cyclic_buffer::get_prev_rail(int n) {
    return _buffer[mod(_index-(n*_incr), _size)];
}

void Cyclic_buffer::reverse() {
    _index = _index - ((_size-1)*_incr);
    _incr = _incr * -1;
}