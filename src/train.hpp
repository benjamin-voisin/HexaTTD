#pragma once

#include <memory>
#include "rail.hpp"

class Train {
	private:
		Rail* _position;
		std::vector<Rail*> _path;
		std::shared_ptr<Texture2D> _sprite;
		float _orientation;
		float _current_speed;
		float _max_speed;
		float _progression; // Progression of the train through the rail.

	public:
		void draw(Layout layout);
		Train(std::vector<Rail*> path);
		void update();
};
