#pragma once

#include <memory>
#include <stdlib.h>

#include "rail.hpp"
#include "track_graph/track_graph.hpp"
#include "train/cyclic_track_buffer.hpp"
#include "wagon.hpp"

class Grid;

class Train {
  protected:
    Cyclic_buffer _previous_rail;
    std::shared_ptr<Texture2D> _sprite;

    float _orientation;
    float _current_speed;
    float _max_speed;

    int _rail_id;
    
    float _progression; // Progression of the train through the rail.
    int _direction;

    virtual void next_rail(Grid* grid);

    std::vector<std::unique_ptr<Wagon>> _wagons;

  public:
    void draw(Layout layout, std::vector<Rail>);
    void reverse();
    Train(int track_id, std::size_t size);
    virtual void update(Grid* grid);
    virtual ~Train();
};

class ItineraryTrain : public Train {
  private:
    std::vector<int> _path;
    int _position;
    void next_rail(Grid* grid) override;

  public:
    ItineraryTrain(std::vector<int> path, std::size_t size);
    virtual ~ItineraryTrain();
};
