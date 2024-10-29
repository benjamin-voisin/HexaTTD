#include <assert.h>

#include "../raylib/src/raylib.h"
/* #include "layout.hpp" */
#include "grid.hpp"

int main() {
	InitWindow(1000, 1000, "HexaTTD");

	Grid grid = Grid(
			layout_flat,
			Vector2 { 100, 75},
			Vector2 {(float) GetScreenWidth() / 2, (float) GetScreenHeight() / 2},
			-3, 3, -3, 3);

	SetTargetFPS(60);

	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		grid.draw();
		DrawFPS(10, 10);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
