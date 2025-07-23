#pragma once

#include "guielement.hpp"
#include "clay.h"

#include <string>

class Slider : GuiElement {
	private:
		Clay_ElementId _id;

	public:
		Slider(std::string id);
		void draw() override;
};
