#include <assert.h>
#include <cstdio>
#include <set>

#include "raylib.h"
#include "grid.hpp"
#include "rail.hpp"
#include "vector.hpp"
#include "train.hpp"

#define WHEEL_FACTOR 5
const char dotfile[20] = "graph.dot" ;

float max(float a, float b) {
	return (a > b) ? a : b;
}

void pp_int_vector(FILE* f, std::vector<int> v) {
	fprintf(f, "[");
	for (long unsigned i=0; i<v.size(); ++i) {
		if (i > 0)
			fprintf(f, ", ");
		fprintf(f, "%d", v[i]);
	}
	fprintf(f, "]\n");
}

int main() {
	char* texte = (char*) malloc(1000 * sizeof(char));
	InitWindow(1000, 1000, "HexaTTD");

	Grid grid1 = Grid(
			layout_flat,
			Vector2 { 100, 100},
			Vector2 {(float) GetScreenWidth() / 2, (float) GetScreenHeight() / 2},
			-10, 10, -10, 10);

	grid1.add_rail(Hex(0,0), 1, 5, 5);
	grid1.add_rail(Hex(1,-1), 2, 5, 5);
	grid1.add_rail(Hex(1,-1) + Hex(1,-1), 2, 0, 5);
	grid1.add_train(new ItineraryTrain({0,1,2}));
	grid1.add_train(new Train(0));

	grid1.add_station(0, "Test");

	Hex last_cursor =     grid1.xy_to_hex(GetMouseX(), GetMouseY());
	Hex last_cursor_pers =     grid1.xy_to_hex(GetMouseX(), GetMouseY());
	
	Hex start_construct = grid1.xy_to_hex(GetMouseX(), GetMouseY());

	SetTargetFPS(60);

	while(!WindowShouldClose()) {
		grid1.update();
		Hex under_cursor = grid1.xy_to_hex(GetMouseX(), GetMouseY());
		

		if (under_cursor != last_cursor)
			last_cursor_pers = last_cursor;
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			start_construct = under_cursor;	
		}

		// Change the zoom level
		auto wheel_move = GetMouseWheelMoveV().y;
		auto new_size = Vector(
			max(5, (grid1.layout->size.x + wheel_move * WHEEL_FACTOR)),
			max(5, grid1.layout->size.y + wheel_move * WHEEL_FACTOR)
		);
		if (new_size.x != grid1.layout->size.x && new_size.y != grid1.layout->size.y) {
			// Coordinates of the cursor :
			Vector cursor = Vector(
				GetMouseX() - grid1.layout->origin.x,
				GetMouseY() - grid1.layout->origin.y

			);
			// It’s new position after offset will be
			Vector new_center = Vector(
				cursor.x * (new_size.x / grid1.layout->size.x),
				cursor.y * (new_size.x / grid1.layout->size.x)
			);
			Vector offset = new_center - cursor;
			grid1.layout->origin.x -= offset.x;
			grid1.layout->origin.y -= offset.y;
			grid1.layout->size.x = new_size.x;
			grid1.layout->size.y = new_size.y;
		}

		// Build rails
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			if (last_cursor_pers.is_neighbor(start_construct) && last_cursor_pers.is_neighbor(under_cursor)) {
				if ((start_construct != last_cursor_pers) && (start_construct != under_cursor)) {
					Hex diff_src = start_construct - last_cursor_pers;
					Hex diff_dst = under_cursor - last_cursor_pers;
					if (   (diff_src.direction() != diff_dst.direction()) 
						&& !(start_construct.is_neighbor(under_cursor)) ) {
						grid1.add_rail(last_cursor_pers, diff_src.direction(), diff_dst.direction(), 4);
						start_construct = last_cursor_pers;
					}
				}
			}
		}

		// Move the map
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
			Vector2 delta = GetMouseDelta();
			grid1.layout->origin.x += delta.x;
			grid1.layout->origin.y += delta.y;
		}


		BeginDrawing();
		ClearBackground(WHITE);
		grid1.draw();
		
		
		
		DrawFPS(10, 10);
		
		grid1.hightlight(under_cursor, GREEN);

		Vector pos = {(float) GetMouseX(), (float) GetMouseY()};
		
		Tile* t = grid1.tile_from_hex(under_cursor);
		std::set<int> on_tile_tracks = t->get_rails_on_tile();
		std::vector<int> selected_rails = {};
		for (auto n = on_tile_tracks.begin(); n != on_tile_tracks.end(); ++n) {
			Rail r = grid1.get_rail(*n);
			if (r.is_on_track(*grid1.layout, pos)) {
				selected_rails.push_back(*n);
			}
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			for (long unsigned i=0; i<selected_rails.size(); ++i) {
				grid1.del_rail(selected_rails[i]);
			}
		} else {
			for (long unsigned i=0; i<selected_rails.size(); ++i) {
				Rail r = grid1.get_rail(selected_rails[i]);
				if (!r.deleted)
					r.draw(*grid1.layout, ORANGE, 1);
			}
		}
#ifndef NDEBUG
		FILE* f = fmemopen(texte, 1000, "w");
		grid1.hightlight(last_cursor_pers, BLUE);
		grid1.hightlight(start_construct, BLACK);
		fprintf(f, "selected_rails= ");
		pp_int_vector(f, selected_rails);
		fprintf(f, "layout.x=%.2f\n", grid1.layout->size.x);
		t->pp(f);
		fclose(f);
		DrawText(texte, 10, 40, 30, BLACK);
#endif
		EndDrawing();
		last_cursor = under_cursor;
	}
	CloseWindow();
	grid1.graph.to_dot(dotfile);
	free(texte);
	return 0;
}
