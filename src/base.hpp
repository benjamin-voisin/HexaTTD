#pragma once

#include "vector.hpp"

class Base {
	private:
		float alpha;

    public:
        Base(float alpha);

        Vector projection(Vector v);
};

