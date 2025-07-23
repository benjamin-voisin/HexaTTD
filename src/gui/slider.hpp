#pragma once

#include "guielement.hpp"
#include "clay.h"

#include <string>

class Slider : public GuiElement {
	private:
		Clay_ElementId _id;

	public:
		Slider(std::string id);
		void draw() override;
		~Slider() override;
};
