#include "slider.hpp"

#include "raylib.h"
#include "clay.h"

Slider::Slider(std::string id) {
	Clay_String string = {false, static_cast<int32_t>(id.length()), id.c_str()};
	_id = CLAY_SID(string);
}

void Slider::draw() {
	auto elementData = Clay_GetElementData(_id);
	DrawRectangle(elementData.boundingBox.x, elementData.boundingBox.y, elementData.boundingBox.width, elementData.boundingBox.height, BLACK);
}

Slider::~Slider() {};
