#include "station.hpp"

Station::Station(Rail *rail, std::string name): _rail{rail}, _name{name} {
}

void Station::draw(Layout layout) {
	auto center = _rail->get_hex().center(layout);
	auto position = _rail->get_position(layout, 0.5f);
	auto pos = position.position + position.direction.orthogonal()*layout.size.x/4;
	DrawCircleV(pos.to_Vector2(), 10, GREEN);
	DrawText(&_name[0], pos.x, pos.y, layout.size.x / 3, GREEN);
}
