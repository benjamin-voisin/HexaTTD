#include "gui.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-result"
#pragma GCC diagnostic ignored "-Wenum-compare"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#pragma GCC diagnostic pop

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
