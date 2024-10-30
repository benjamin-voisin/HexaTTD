#include "rail.hpp"
#include "../raylib/src/raylib.h"
#include "vector.hpp"

#include <stdio.h>


void Rail::draw(Layout layout) {
	Vector center = Vector(_tile.center(layout));
	std::vector<Vector2> corners = _tile.corners(layout);

	Vector c1 = Vector(corners[_entry_point]);
	Vector c2 = Vector(corners[_entry_point  + 1]);
	Base b1 = _tile.base_sides(layout, _entry_point-1 % 6);
	Vector c3 = Vector(corners[_exit_point]);
	Vector c4 = Vector(corners[_exit_point + 1]);
	Base b2 = _tile.base_sides(layout, _exit_point-1 % 6);

	Vector2 p1[3];

	Vector coffset_center_src = Vector(GetSplinePointLinear(c1.to_Vector2(), c2.to_Vector2(), 0.50));
	Vector coffset_droite_src = Vector(GetSplinePointLinear(c1.to_Vector2(), c2.to_Vector2(), 0.25));
	Vector coffset_gauche_src = Vector(GetSplinePointLinear(c1.to_Vector2(), c2.to_Vector2(), 0.75));

	Vector coffset_center_dst = Vector(GetSplinePointLinear(c3.to_Vector2(), c4.to_Vector2(), 0.50));
	Vector coffset_droite_dst = Vector(GetSplinePointLinear(c3.to_Vector2(), c4.to_Vector2(), 0.75));
	Vector coffset_gauche_dst = Vector(GetSplinePointLinear(c3.to_Vector2(), c4.to_Vector2(), 0.25));

	Vector offset_droite_src = coffset_droite_src-coffset_center_src;
	Vector offset_droite_dst = coffset_droite_dst-coffset_center_dst;

	Vector offset_gauche_src = coffset_gauche_src-coffset_center_src;
	Vector offset_gauche_dst = coffset_gauche_dst-coffset_center_dst;


	DrawLineEx(center.to_Vector2(), (center+offset_droite_src).to_Vector2(), 2, GREEN);
	DrawLineEx(center.to_Vector2(), (center+offset_droite_dst).to_Vector2(), 2, BLUE);
	DrawLineEx(center.to_Vector2(), (center+offset_droite_src.proj(offset_droite_dst)).to_Vector2(), 2, GREEN);
	DrawLineEx(center.to_Vector2(), (center+offset_droite_dst.proj(offset_droite_src)).to_Vector2(), 2, BLUE);
	
	Vector pt_controle_droite = center + offset_droite_dst.proj(offset_droite_src) + offset_droite_src.proj(offset_droite_dst);

	p1[1] = pt_controle_droite.to_Vector2();
	p1[0] = coffset_droite_src.to_Vector2();
	p1[2] = coffset_droite_dst.to_Vector2();

	Vector pt_controle_gauche = center + offset_gauche_dst.proj(offset_gauche_src) + offset_gauche_src.proj(offset_gauche_dst);
	Vector2 p2[3];
	p2[1] = pt_controle_gauche.to_Vector2();
	p2[0] = coffset_gauche_src.to_Vector2();
	p2[2] = coffset_gauche_dst.to_Vector2();


	DrawCircleV(p1[1], 2, RED);
	DrawSplineBezierQuadratic(p1, 3, _width, BLACK);
	DrawSplineBezierQuadratic(p2, 3, _width, BLACK);
}
