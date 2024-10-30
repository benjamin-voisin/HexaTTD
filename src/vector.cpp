#include "vector.hpp"

#include <math.h>
Vector::Vector(float x, float y) : x{x}, y{y} {};

Vector::Vector(Vector2 v) : Vector(v.x, v.y) {};

bool operator == (Vector a, Vector b) {
	return (a.x == b.x) && (a.y == b.y);
}

bool operator != (Vector a, Vector b) {
	return !(a == b);
}

Vector operator + (Vector a, Vector b) {
	return Vector(a.x + b.x, a.y + b.y);
}

Vector operator - (Vector a, Vector b) {
	return Vector(a.x - b.x, a.y - b.y);
}
float operator * (Vector a, Vector b) {
	return a.x * b.x + a.y * b.y;
}
Vector operator * (Vector a, float k) {
	return Vector(a.x*k, a.y*k);
}
Vector operator / (Vector a, float k) {
	return Vector(a.x/k, a.y/k);
}
float Vector::norme() {
	return sqrt(x * x + y * y);
}
Vector Vector::normalise() {
	return *this / (this->norme());
}

Vector Vector::proj(Vector a) {
	return this->normalise() * (this->normalise() * a);
}

Vector2 Vector::to_Vector2() {
    return Vector2{x, y};
}