#include "base.hpp"

#include <math.h>

Base::Base(float alpha) : alpha{alpha} {};

Vector Base::projection(Vector v) {
    return Vector{
              - cos(this->alpha) * v.x + sin(this->alpha) * v.y,
              - sin(this->alpha) * v.x - cos(this->alpha) * v.y};
}
