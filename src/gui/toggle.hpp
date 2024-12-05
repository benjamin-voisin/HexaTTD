#pragma once
#include "gui.hpp"

#include <string>

class GuiToggleC : GuiElement {
	private:
		bool _pressed;
		std::string _text;

	public:
		void draw() override;
		bool is_pressed();
		GuiToggleC(float x, float y, float width, float height, std::string text);
};
