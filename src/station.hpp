#pragma once

#include "rail.hpp"
#include <string>

class Station {
	private:
		Rail *_rail;
		std::string _name;
	public:
		Station(Rail *rail, std::string name);
		void draw(Layout layout);
};
