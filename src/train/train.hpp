#pragma once

#include <memory>
#include <mutex>
#include <stdlib.h>

#include "../log.hpp"
#include "../rail.hpp"
#include "cyclic_track_buffer.hpp"
#include "wagon.hpp"

class Grid;

class Train {
  protected:
    std::mutex _lock;
    Cyclic_buffer _previous_rail;
    std::shared_ptr<Texture2D> _sprite;

    int _id;
    std::size_t _size;

    float _progression; // Progression of the train through the rail.

    virtual void next_rail(Grid *grid);

    std::vector<std::unique_ptr<Wagon>> _wagons;

  public:
    void draw(Layout *layout, std::vector<Rail> &rails);
    void reverse();
    Train(Grid *grid, int id, int track_id, std::size_t size);
    virtual void update(Grid *grid);
    virtual ~Train();
    Train &operator>>(Log::Logger *log);
};

class ItineraryTrain : public Train {
  private:
    std::vector<int> _path;
    int _position;
    void next_rail(Grid *grid) override;

  public:
    ItineraryTrain(Grid *grid, int id, std::vector<int> path, std::size_t size);
    virtual ~ItineraryTrain();
};
