// This file will contain the implementation of Raygui, and only this file
#define RAYGUI_IMPLEMENTATION
#include "gui.hpp"

GuiToggleC::GuiToggleC(float x, float y, float width, float heigt, std::string text) :
	_x{x}, _y{y}, _width{width}, _height{heigt}, _text{text} {
		_pressed = false;
}

void GuiToggleC::draw() {
	GuiToggle((Rectangle){_x, _y, _width, _height}, _text.c_str(), &_pressed);
}

bool GuiToggleC::is_pressed() {
	return _pressed;
}
