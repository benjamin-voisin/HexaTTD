#pragma once

#include <string>

#include "raygui.h"

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
