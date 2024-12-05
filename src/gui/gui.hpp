#pragma once

// We ignore a bunch of warning when including raygui...
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-result"
#pragma GCC diagnostic ignored "-Wenum-compare"
#include "raygui.h"
#pragma GCC diagnostic pop


class GuiElement {
	protected:
		float _x;
		float _y;
		float _width;
		float _height;
	public:
		GuiElement(float x, float y, float width, float height);
		virtual void draw();
};
