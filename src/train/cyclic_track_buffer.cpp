#include "cyclic_track_buffer.hpp"

#include <assert.h>

int mod(int a, int b) {
    return ((a % b) + b) % b;
}

Cyclic_buffer::Cyclic_buffer(std::size_t sizemax) {
    _sizemax = sizemax;
    _size = 0;
    _index = 0;
    _incr = 1;
    _buffer = (prev_rail_s*) calloc(sizemax, sizeof(prev_rail_s));
    
}

Cyclic_buffer::~Cyclic_buffer() {
    free(_buffer);
}

std::size_t Cyclic_buffer::index_min() {
    return _index - ((_size-1)*_incr);
}

void Cyclic_buffer::add_prev_rail(int rail_id, int direction) {
    assert(_size < _sizemax);
    _index = (_index + _incr) % _size;
    _buffer[_index].rail_id = rail_id;
    _buffer[_index].direction = direction;
    _size++;
}

prev_rail_s Cyclic_buffer::del_last_prev_rail() {
    assert(_size > 0);
    prev_rail_s ret = get_prev_rail(_size-1);
    _size--;
    return ret;
}

prev_rail_s Cyclic_buffer::get_prev_rail(int n) {
    assert((0 <= n) && (n < _size));
    return _buffer[mod(_index-(n*_incr), _size)];
}

void Cyclic_buffer::reverse() {
    _index = this->index_min();
    _incr = _incr * -1;
}