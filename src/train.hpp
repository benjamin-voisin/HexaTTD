#pragma once

#include <memory>

#include "rail.hpp"
#include "track_graph/track_graph.hpp"

class Train {
	protected:
		std::shared_ptr<Texture2D> _sprite;

		float _orientation;
		float _current_speed;
		float _max_speed;

		int _rail_id;
		float _progression; // Progression of the train through the rail.
		int _direction;

		virtual void next_rail(Graph graph, std::vector<Rail> rails);

	public:
		void draw(Layout layout, std::vector<Rail>);
		Train(int track_id);
		virtual void update(Graph graph, std::vector<Rail> rails);
};

class ItineraryTrain : public Train {
	private:
		std::vector<int> _path;
		int _position;
		void next_rail(Graph graph, std::vector<Rail> rails) override;
	
	public:
		ItineraryTrain(std::vector<int> path);
};
