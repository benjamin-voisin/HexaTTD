#pragma once

#include <string>

// We ignore a bunch of warning when including raygui...
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-result"
#pragma GCC diagnostic ignored "-Wenum-compare"
#include "raygui.h"
#pragma GCC diagnostic pop

class GuiToggleC {
	private:
		float _x;
		float _y;
		float _width;
		float _height;
		std::string _text;
		bool _pressed;

	public:
		void draw();
		bool is_pressed();
		GuiToggleC(float x, float y, float width, float height, std::string text);
};
