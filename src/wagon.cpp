#include "wagon.hpp"
#include "graphics/display_train.hpp"

void Wagon::draw(Layout layout, std::vector<Rail> rails, int rail_id, float _progression) {
	if (rails[rail_id].get_hex().is_visible(layout)) {
		auto position = rails[rail_id].get_position(layout, _progression);
		DrawTrain train = DrawTrain(position.position, Vector(30, 70) * layout.size.x / 100, position.direction);
		train.draw();
	}
}

Wagon::Wagon(std::string type, float capacity)
	: _capacity{capacity}, _type{type} {
}

Locomotive::Locomotive(): Wagon("locomotive", 0.0) {
}

void Locomotive::draw(Layout layout, std::vector<Rail> rails, int rail_id, float _progression) {
	if (rails[rail_id].get_hex().is_visible(layout)) {
		auto position = rails[rail_id].get_position(layout, _progression);
		DrawTrain train = DrawTrain(position.position, Vector(30, 70) * layout.size.x / 100, position.direction);
		train.draw();
	}
}


Wagon::~Wagon() { }
Locomotive::~Locomotive() { }
