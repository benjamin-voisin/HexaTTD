#pragma once

#include <memory>
#include "rail.hpp"

class Train {
	private:
		int _position;
		std::vector<int> _path;
		std::shared_ptr<Texture2D> _sprite;
		float _orientation;
		float _current_speed;
		float _max_speed;
		float _progression; // Progression of the train through the rail.

	public:
		void draw(Layout layout, std::vector<Rail>);
		Train(std::vector<int> path);
		void update(std::vector<Rail> rails);
};
