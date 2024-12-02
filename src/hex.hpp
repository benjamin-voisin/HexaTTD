#pragma once

#include <vector>

#include "base.hpp"
#include "layout.hpp"
#include "raylib.h"

class Hex {
  private:
    // We store the coordinates of the hex in evry possible way, we will see
    // what fits the most our needs
    int _q;
    int _r;
    int _s;
    int coordinates[3];

    Hex add(Hex a);
    Hex subtract(Hex a);
    Hex multiply(int k);

  public:
    void draw(Layout layout, Color color);
    Hex(int q, int r, int s);
    // As q + r + s = 0, we can build a constructor that computes s
    Hex(int q, int r);
    Hex(Vector v);

    int get_q();
    int get_r();
    int get_s();

    friend bool operator==(Hex a, Hex b);
    friend bool operator!=(Hex a, Hex b);
    friend Hex operator+(Hex a, Hex b);
    friend Hex operator-(Hex a, Hex b);
    friend Hex operator*(Hex a, int k);

    int length();
    int distance(Hex a);
    Vector2 center(Layout layout);
    Vector to_Vector();
    Base base_sides(Layout layout, int i);
    std::vector<Vector2> corners(Layout layout);
    Vector2 center_side(Layout layout, int direction);
    Vector2 corner(Layout layout, int corner);
    static float corner_angle(Layout layout, int corner);
    bool is_neighbor(Hex other);
    Hex neighbor(int direction);
    static int opposite_direction(int direction);
    int direction();
    bool is_visible(Layout layout);
};

Hex hex_direction(int direction);

/* #ifdef TEST */
#include "test.hpp"
class HexTest : public TestInstance {
	public:
		void run_test() override;
		HexTest();
		virtual ~HexTest();
};

/* #endif // TEST */
