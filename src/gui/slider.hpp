#pragma once

#include "guielement.hpp"
#include "clay.h"

#include <string>

class Slider : public GuiElement {
	private:
		float _current;
		float _min;
		float _max;
		std::string _value_text;
	public:
		Slider(std::string id, float current, float min, float max);
		float click(float x);
		void draw() override;
		~Slider() override; // Update the slider state and return the new value
};
