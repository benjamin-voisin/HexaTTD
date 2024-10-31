#include <assert.h>
#include <cstdio>

#include "../raylib/src/raylib.h"
#include "grid.hpp"
#include "rail.hpp"

int main() {
	InitWindow(1000, 1000, "HexaTTD");

	Grid grid1 = Grid(
			layout_flat,
			Vector2 { 100, 100},
			Vector2 {(float) GetScreenWidth() / 2, (float) GetScreenHeight() / 2},
			-10, 10, -10, 10);

	Rail rail = Rail(Hex(0,0), 1, 5, 5);
	Rail rail1 = Rail(Hex(1,-1), 2, 4, 5);

	SetTargetFPS(60);

	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(WHITE);
		grid1.draw();
		Hex under_cursor = grid1.xy_to_hex(GetMouseX(), GetMouseY());
		grid1.hightlight(under_cursor);
		rail1.draw(*grid1.layout);
		DrawFPS(10, 10);
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
			Vector2 delta = GetMouseDelta();
			grid1.layout->origin.x += delta.x;
			grid1.layout->origin.y += delta.y;
		}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
