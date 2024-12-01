#pragma once

#include <memory>

#include "rail.hpp"
#include "wagon.hpp"
#include "track_graph/track_graph.hpp"

class Train {
	protected:
		std::shared_ptr<Texture2D> _sprite;

		float _orientation;
		float _current_speed;
		float _max_speed;


		int _rail_id;
		int *_prev_rails;
		size_t _prev_rails_index;
		size_t _prev_rails_size;
		void add_prev_rail(int rail_id, int direction);
		int get_prev_rail(int n);
		float _progression; // Progression of the train through the rail.
		int _direction;

		virtual void next_rail(Graph graph, std::vector<Rail> rails);

		std::vector<std::unique_ptr<Wagon>> _wagons;

	public:
		void draw(Layout layout, std::vector<Rail>);
		Train(int track_id);
		virtual void update(Graph graph, std::vector<Rail> rails);
		virtual ~Train();
};

class ItineraryTrain : public Train {
	private:
		std::vector<int> _path;
		int _position;
		void next_rail(Graph graph, std::vector<Rail> rails) override;
	
	public:
		ItineraryTrain(std::vector<int> path);
		virtual ~ItineraryTrain();
};
