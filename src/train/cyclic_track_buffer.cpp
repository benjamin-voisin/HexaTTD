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

std::size_t Cyclic_buffer::get_size() {
    return _size;
}

std::size_t Cyclic_buffer::index_min() {
    return mod(_index - ((_size-1)*_incr), _sizemax);
}

void Cyclic_buffer::add_prev_rail(int rail_id, int direction) {
    assert(_size < _sizemax);
    _index = mod(_index + _incr, _sizemax);
    _buffer[_index].rail_id = rail_id;
    _buffer[_index].direction = direction * _incr;
    _size++;
}

prev_rail_s Cyclic_buffer::del_last_prev_rail() {
    assert(_size > 0);
    prev_rail_s ret = get_prev_rail(_size-1);
    _size--;
    return ret;
}

prev_rail_s Cyclic_buffer::get_prev_rail(std::size_t n) {
    assert((long unsigned) n < _size);
    prev_rail_s ret = _buffer[mod(_index-(n*_incr), _sizemax)];
    ret.direction *= _incr;
    return ret;
}

void Cyclic_buffer::reverse() {
    _index = this->index_min();
    _incr = _incr * -1;
}

void Cyclic_buffer::pp(FILE* f) {
    fprintf(f, "[");
    for (std::size_t i=0; i<get_size(); ++i) {
        if (i > 0)
            fprintf(f, ", ");
        prev_rail_s s = get_prev_rail(i);
        fprintf(f, "(d=%d, r=%d)", s.direction, s.rail_id);
    }
    fprintf(f, "]");
}