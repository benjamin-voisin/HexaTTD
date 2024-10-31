#pragma once

#include "../raylib/src/raylib.h"

class Vector {
	private:
		

    public:
        float x;
        float y;
        Vector(Vector2 v);
        Vector(float x, float y);

        friend bool operator == (Vector a, Vector b);
		friend bool operator != (Vector a, Vector b);
		friend Vector operator + (Vector a, Vector b);
		friend Vector operator - (Vector a, Vector b);
        friend float operator * (Vector a, Vector b);
        
        friend Vector operator * (Vector a, float k);
        friend Vector operator / (Vector a, float k);

        float norme();
        Vector normalise();
        Vector orthogonal();
        Vector proj(Vector a);
        Vector2 to_Vector2();
};