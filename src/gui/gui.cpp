// This file will contain the implementation of Raygui, and only this file
#define RAYGUI_IMPLEMENTATION
#include "gui.hpp"

GuiElement::GuiElement(float x, float y, float width, float heigt) :
	_x{x}, _y{y}, _width{width}, _height{heigt} { }

void GuiElement::draw() { }
