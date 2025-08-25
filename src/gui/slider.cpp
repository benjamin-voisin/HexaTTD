#include "slider.hpp"

#include "raylib.h"
#include "clay.h"

#include <format>

Slider::Slider(std::string ids, float current, float min, float max) : _current{current}, _min{min}, _max{max} {
	Clay_String string = {false, static_cast<int32_t>(ids.length()), ids.c_str()};
	id = CLAY_SID(string);
	_value_text = std::format("{:.0f}%", _current * 100);
}

void Slider::draw() {
	auto elementData = Clay_GetElementData(id);
	int value_size = MeasureText("100%", 20);
	auto v_middle = elementData.boundingBox.y + (elementData.boundingBox.height / 2);
	auto h_text = elementData.boundingBox.x + elementData.boundingBox.width - value_size;
	int padding = 10;
	DrawText(_value_text.c_str(), h_text, v_middle - (elementData.boundingBox.height / 6), 20, {0,0,0, 255});
	DrawRectangle(elementData.boundingBox.x, v_middle - (elementData.boundingBox.height / 6), elementData.boundingBox.width - value_size - padding, elementData.boundingBox.height / 3, BLACK);

	// Draw the current value
	int selector_width = 10;
	int selector_pos = elementData.boundingBox.x + ((elementData.boundingBox.width - value_size - padding) * _current) - (selector_width / 2.);
	DrawRectangle(selector_pos, elementData.boundingBox.y, selector_width, elementData.boundingBox.height, {0,0,0,255});

}

float Slider::click(float x) {
	auto elementData = Clay_GetElementData(id);
	int value_size = MeasureText("100%", 20);
	auto h_text = elementData.boundingBox.x + elementData.boundingBox.width - value_size;
	if (elementData.boundingBox.x <= x && x <= h_text) {
		// The click is on the slider itself
		_current = (x - elementData.boundingBox.x) / (h_text - elementData.boundingBox.x);
		_value_text = std::format("{:.0f}%", _current * 100);
	}
	return _current;
}

Slider::~Slider() {};
