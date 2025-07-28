#include "slider.hpp"

#include "raylib.h"
#include "clay.h"

Slider::Slider(std::string ids) {
	Clay_String string = {false, static_cast<int32_t>(ids.length()), ids.c_str()};
	id = CLAY_SID(string);
}

void Slider::draw() {
	auto elementData = Clay_GetElementData(id);
	DrawRectangle(elementData.boundingBox.x, elementData.boundingBox.y, elementData.boundingBox.width, elementData.boundingBox.height, BLACK);
}

Slider::~Slider() {};
