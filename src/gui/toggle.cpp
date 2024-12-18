#include "toggle.hpp"
#include "gui.hpp"

GuiToggleElement::GuiToggleElement(float x, float y, float width, float height,
                                   std::string text, bool init_value)
    : GuiElement(x, y, width, height), _pressed{init_value}, _text{text} {}

void GuiToggleElement::draw() {
    GuiToggle((Rectangle){_x, _y, _width, _height}, _text.c_str(), &_pressed);
}

bool GuiToggleElement::is_pressed() { return _pressed; }
