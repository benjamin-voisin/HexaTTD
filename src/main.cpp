#include <assert.h>

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

	Rail rail = Rail(Hex(0,0), 0, 0, 10);

	SetTargetFPS(60);

	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(WHITE);
		grid1.draw();
		rail.draw(*grid1.layout);
		DrawFPS(10, 10);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
