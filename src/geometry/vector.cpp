#include "vector.hpp"

#include <cmath>
#include <math.h>
Vector::Vector(float x, float y) : x{x}, y{y} {};

Vector::Vector(Vector2 v) : Vector(v.x, v.y) {};

Vector::Vector(float angle) {
    x = cos(angle);
    y = sin(angle);
}

bool operator==(Vector a, Vector b) { return (a.x == b.x) && (a.y == b.y); }

bool operator!=(Vector a, Vector b) { return !(a == b); }

Vector operator+(Vector a, Vector b) { return Vector(a.x + b.x, a.y + b.y); }

Vector operator-(Vector a, Vector b) { return Vector(a.x - b.x, a.y - b.y); }
float operator*(Vector a, Vector b) { return a.x * b.x + a.y * b.y; }
Vector operator*(Vector a, float k) { return Vector(a.x * k, a.y * k); }
Vector operator/(Vector a, float k) { return Vector(a.x / k, a.y / k); }
float Vector::norme() { return sqrt(x * x + y * y); }
Vector Vector::orthogonal() { return Vector(-y, x); }
Vector Vector::normalise() { return *this / (this->norme()); }

Vector Vector::proj(Vector a) {
    return this->normalise() * (this->normalise() * a);
}

Vector2 Vector::to_Vector2() { return Vector2{x, y}; }

constexpr float rad_to_deg(float rad) { return rad * (180 / M_PI); }

float Vector::angle() {
    if (x == 0) // special cases
        return (y > 0) ? 180 : (y == 0) ? 90 : 0;
    else if (y == 0) // special cases
        return (x >= 0) ? 90 : 270;
    float ret = rad_to_deg(atanf(y / x));
    if (x < 0 && y < 0) // quadrant Ⅲ
        ret = 180 + ret;
    else if (x < 0)             // quadrant Ⅱ
        ret = 180 + ret;        // it actually substracts
    else if (y < 0)             // quadrant Ⅳ
        ret = 270 + (90 + ret); // it actually substracts
    return ret + 90;
}
