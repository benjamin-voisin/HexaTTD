#include <assert.h>
#include <cstdio>

#include "../raylib/src/raylib.h"
#include "grid.hpp"
#include "rail.hpp"

#define WHEEL_FACTOR 5

float max(float a, float b) {
	return (a > b) ? a : b;
}


int main() {
	char* texte = (char*) malloc(100 * sizeof(char));
	InitWindow(1000, 1000, "HexaTTD");

	Grid grid1 = Grid(
			layout_flat,
			Vector2 { 100, 100},
			Vector2 {(float) GetScreenWidth() / 2, (float) GetScreenHeight() / 2},
			-10, 10, -10, 10);

	Rail rail = Rail(Hex(0,0), 1, 5, 5);
	Rail rail1 = Rail(Hex(1,-1), 2, 4, 5);
	Rail rail2 = Rail(Hex(1,0), 1, 4, 5);
	Rail rail3 = Rail(Hex(1,1), 1, 3, 5);

	std::vector<Rail> rails = {rail, rail1, rail2, rail3};

	Hex last_cursor =     grid1.xy_to_hex(GetMouseX(), GetMouseY());
	Hex last_cursor_pers =     grid1.xy_to_hex(GetMouseX(), GetMouseY());
	
	Hex start_construct = grid1.xy_to_hex(GetMouseX(), GetMouseY());

	SetTargetFPS(60);

	while(!WindowShouldClose()) {
		Hex under_cursor = grid1.xy_to_hex(GetMouseX(), GetMouseY());
		if (under_cursor != last_cursor)
			last_cursor_pers = last_cursor;
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			start_construct = under_cursor;	
		}
		auto wheel_move = GetMouseWheelMoveV().y;
		grid1.layout->size.x = max(5, grid1.layout->size.x + wheel_move * WHEEL_FACTOR);
		grid1.layout->size.y = max(5, grid1.layout->size.y + wheel_move * WHEEL_FACTOR);
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			if (last_cursor_pers.is_neighbor(start_construct) && last_cursor_pers.is_neighbor(under_cursor)) {
				if ((start_construct != last_cursor_pers) && (start_construct != under_cursor)) {
					Hex diff_src = start_construct - last_cursor_pers;
					Hex diff_dst = under_cursor - last_cursor_pers;
					if (   (diff_src.direction() != diff_dst.direction()) 
						&& (diff_src.direction() != 1+diff_dst.direction())
						&& (diff_src.direction()+1 != diff_dst.direction()) ) {
						Rail r = Rail(last_cursor_pers, diff_src.direction(), diff_dst.direction(), 4);
						rails.push_back(r);
						start_construct = last_cursor_pers;
					}
				}
			}
		}

		BeginDrawing();
		ClearBackground(WHITE);
		grid1.draw();
		for (auto r = rails.begin(); r != rails.end(); ++r) {
			r->draw(*grid1.layout);
		}
		
		
		DrawFPS(10, 10);
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
			Vector2 delta = GetMouseDelta();
			grid1.layout->origin.x += delta.x;
			grid1.layout->origin.y += delta.y;
		}

		
		grid1.hightlight(under_cursor, GREEN);
		grid1.hightlight(last_cursor_pers, BLUE);
		grid1.hightlight(start_construct, BLACK);

		
		snprintf(texte, 100, "layout.x=%.2f", grid1.layout->size.x);
		DrawText(texte, 10, 40, 30, BLACK);
		EndDrawing();
		last_cursor = under_cursor;
	}
	CloseWindow();
	free(texte);
	return 0;
}
