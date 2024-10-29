#include <assert.h>

#include "../raylib/src/raylib.h"
#include "hex.hpp"
#include "grid.hpp"

int main() {
	InitWindow(1000, 1000, "feur");

	Hex hexagone1 = Hex(0,0,0, 100);
	Grid grid = Grid(layout_flat, Vector2 { 100, 100}, Vector2 {(float) GetScreenWidth() / 2, (float) GetScreenHeight() / 2});

	SetTargetFPS(24);

	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		hexagone1.draw(grid);
		(hexagone1 + hex_direction(0)).draw(grid);
		(hexagone1 + hex_direction(1)).draw(grid);
		(hexagone1 + hex_direction(2)).draw(grid);
		(hexagone1 + hex_direction(3)).draw(grid);
		(hexagone1 + hex_direction(4)).draw(grid);
		(hexagone1 + hex_direction(5)).draw(grid);
		EndDrawing();
		grid.origin.x += 1;
		grid.origin.y += 1;
	}
	CloseWindow();
	return 0;
}
