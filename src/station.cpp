#include "station.hpp"

Station::Station(int rail_id, std::string name)
    : _rail_id{rail_id}, _name{name} {}

void Station::draw(Layout *layout, std::vector<Rail> rails) {
    if (rails[_rail_id].get_hex().is_visible(layout)) {
        auto position = rails[_rail_id].get_position(layout, 0.5f);
        auto pos = position.position +
                   position.direction.orthogonal() * layout->size.x / 4;
        DrawCircleV(pos.to_Vector2(), 10, GREEN);
        DrawText(&_name[0], pos.x, pos.y, layout->size.x / 3, GREEN);
    }
}
