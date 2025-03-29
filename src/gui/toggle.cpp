#include "toggle.hpp"
#include "gui.hpp"

GuiToggleElement::GuiToggleElement(float x, float y, float width, float height,
                                   std::string text, bool init_value)
    : GuiElement(x, y, width, height), _pressed{init_value}, _text{text} {}

void GuiToggleElement::draw() {}

bool GuiToggleElement::is_pressed() { return _pressed; }
