#include "slider.hpp"

#include "raylib.h"
#include "clay.h"

#include <format>

Slider::Slider(std::string ids, float current, float min, float max) : _current{current}, _min{min}, _max{max} {
	Clay_String string = {false, static_cast<int32_t>(ids.length()), ids.c_str()};
	id = CLAY_SID(string);
}

void Slider::draw() {
	auto elementData = Clay_GetElementData(id);
	std::string value = std::format("{:.0f}%", _current * 100);
	int value_size = MeasureText(value.c_str(), 20);
	auto v_middle = elementData.boundingBox.y + (elementData.boundingBox.height / 2);
	auto h_text = elementData.boundingBox.x + elementData.boundingBox.width - value_size;
	int padding = 10;
	DrawText(value.c_str(), h_text, v_middle - (elementData.boundingBox.height / 6), 20, {0,0,0, 255});
	DrawRectangle(elementData.boundingBox.x, v_middle - (elementData.boundingBox.height / 6), elementData.boundingBox.width - value_size - padding, elementData.boundingBox.height / 3, BLACK);

	// Draw the current value
	int selector_width = 10;
	int selector_pos = elementData.boundingBox.x + ((elementData.boundingBox.width - value_size - padding) * _current) - (selector_width / 2.);
	DrawRectangle(selector_pos, elementData.boundingBox.y, selector_width, elementData.boundingBox.height, {0,0,0,255});

}

Slider::~Slider() {};
