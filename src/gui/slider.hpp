#pragma once

#include "guielement.hpp"
#include "clay.h"

#include <string>

class Slider : public GuiElement {
	private:
		float _current;
		float _min;
		float _max;
	public:
		Slider(std::string id, float current, float min, float max);
		void draw() override;
		~Slider() override;
};
