#include "raylib/src/raylib.h"

int main() {
	InitWindow(1000, 1000, "feur");
	while(!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
