#include "toggle.hpp"
#include "gui.hpp"

GuiToggleC::GuiToggleC(float x, float y, float width, float height, std::string text)
	: GuiElement(x, y, width, height), _text{text} { }

void GuiToggleC::draw() {
	GuiToggle((Rectangle){_x, _y, _width, _height}, _text.c_str(), &_pressed);
}

bool GuiToggleC::is_pressed() {
	return _pressed;
}
