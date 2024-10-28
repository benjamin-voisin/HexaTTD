#include "raylib/src/raylib.h"

#include "hex.hpp"

int main() {
	InitWindow(1000, 1000, "feur");

	hex hexagone = hex(0,0,0);

	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		hexagone.draw();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
