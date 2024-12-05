#include "toggle.hpp"
#include "gui.hpp"

GuiToggleElement::GuiToggleElement(float x, float y, float width, float height, std::string text)
	: GuiElement(x, y, width, height), _text{text} { }

void GuiToggleElement::draw() {
	GuiToggle((Rectangle){_x, _y, _width, _height}, _text.c_str(), &_pressed);
}

bool GuiToggleElement::is_pressed() {
	return _pressed;
}
