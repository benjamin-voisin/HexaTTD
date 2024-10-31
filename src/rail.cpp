#include "rail.hpp"
#include "../raylib/src/raylib.h"
#include "vector.hpp"
#include "display_track.hpp"

#include <stdio.h>


void Rail::draw(Layout layout) {
	Vector center = Vector(_tile.center(layout));
	std::vector<Vector2> corners = _tile.corners(layout);

	Vector c1 = Vector(corners[_entry_neighbor]);
	Vector c2 = Vector(corners[_entry_neighbor + 1]);
	Vector c3 = Vector(corners[_exit_neighbor]);
	Vector c4 = Vector(corners[_exit_neighbor + 1]);

	Vector coffset_center_src = Vector(GetSplinePointLinear(c1.to_Vector2(), c2.to_Vector2(), 0.50));
	Vector coffset_droite_src = Vector(GetSplinePointLinear(c1.to_Vector2(), c2.to_Vector2(), 0.25));
	Vector coffset_gauche_src = Vector(GetSplinePointLinear(c1.to_Vector2(), c2.to_Vector2(), 0.75));

	Vector coffset_center_dst = Vector(GetSplinePointLinear(c3.to_Vector2(), c4.to_Vector2(), 0.50));
	Vector coffset_droite_dst = Vector(GetSplinePointLinear(c3.to_Vector2(), c4.to_Vector2(), 0.75));
	Vector coffset_gauche_dst = Vector(GetSplinePointLinear(c3.to_Vector2(), c4.to_Vector2(), 0.25));

	if (_entry_neighbor % 3 == _exit_neighbor % 3) {
		// Si l'on doit faire une ligne droite
		DrawLineEx(coffset_droite_src.to_Vector2(), coffset_droite_dst.to_Vector2(), _width, BLACK);
		DrawLineEx(coffset_gauche_src.to_Vector2(), coffset_gauche_dst.to_Vector2(), _width, BLACK);
	} else {
		// Si il faut calculler un virage
		//assert(_entry_neighbor == _exit_neighbor + 2);
		//assert(_entry_neighbor + 2 == _exit_neighbor);
	
		Hex tile_circle = _tile.neighbor((_entry_neighbor+1) / 2);

		Hex tile_circle2 = _tile.neighbor(3);

		Hex neighbor_src = _tile.neighbor(_entry_neighbor);
		Hex neighbor_dst = _tile.neighbor(_exit_neighbor);

		//DrawLineEx(neighbor_src.center(layout), _tile.center(layout), _width, BLACK);
		//DrawLineEx(neighbor_dst.center(layout), _tile.center(layout), _width, BLACK);

		ArcTrack track = ArcTrack{tile_circle.center(layout), 150, 25, _tile.corner_angle(layout, _exit_neighbor-2) * 180 / M_PI, _tile.corner_angle(layout, _entry_neighbor+3) * 180 / M_PI};
		track.draw();

		ArcTrack track2 = ArcTrack{tile_circle2.center(layout), 150, 25, _tile.corner_angle(layout, _exit_neighbor-2) * 180 / M_PI, _tile.corner_angle(layout, _entry_neighbor+3) * 180 / M_PI};
		track2.draw();

		DrawCircleV(tile_circle.center(layout), 4, GREEN);
		DrawCircleV(tile_circle2.center(layout), 4, RED);
		DrawCircleV(_tile.center(layout), 4, PURPLE);
		
		/*
		Vector2 p1[3];
		
		Vector offset_droite_src = coffset_droite_src-coffset_center_src;
		Vector offset_droite_dst = coffset_droite_dst-coffset_center_dst;

		Vector offset_gauche_src = coffset_gauche_src-coffset_center_src;
		Vector offset_gauche_dst = coffset_gauche_dst-coffset_center_dst;

		DrawLineEx(coffset_droite_src.to_Vector2(), (center+offset_droite_src).to_Vector2(), 2, PURPLE);
		DrawLineEx(coffset_droite_dst.to_Vector2(), (center+offset_droite_dst).to_Vector2(), 2, PURPLE);

		DrawLineEx(center.to_Vector2(), (center+offset_droite_src).to_Vector2(), 2, GREEN);
		DrawLineEx(center.to_Vector2(), (center+offset_droite_dst).to_Vector2(), 2, BLUE);
		DrawLineEx(center.to_Vector2(), (center+offset_droite_src.proj(offset_droite_dst)).to_Vector2(), 2, GREEN);
		DrawLineEx(center.to_Vector2(), (center+offset_droite_dst.proj(offset_droite_src)).to_Vector2(), 2, BLUE);
			
		Vector pt_controle_droite = center + ((offset_droite_dst.proj(offset_droite_src) + offset_droite_src.proj(offset_droite_dst)));

		p1[1] = pt_controle_droite.to_Vector2();
		p1[0] = coffset_droite_src.to_Vector2();
		p1[2] = coffset_droite_dst.to_Vector2();

		Vector pt_controle_gauche = center + offset_gauche_dst.proj(offset_gauche_src) + offset_gauche_src.proj(offset_gauche_dst);
		Vector2 p2[3];
		p2[1] = pt_controle_gauche.to_Vector2();
		p2[0] = coffset_gauche_src.to_Vector2();
		p2[2] = coffset_gauche_dst.to_Vector2();
		

		DrawSplineBezierQuadratic(p1, 3, _width, BLACK);
		DrawSplineBezierQuadratic(p2, 3, _width, BLACK);
		DrawCircleV(p1[1], 4, RED);
		*/
	}
}
