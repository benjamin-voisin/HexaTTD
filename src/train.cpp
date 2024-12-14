#include "train.hpp"
#include "grid.hpp"

#include <algorithm>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

Train::Train(int id, int track_id, std::size_t size) 
    : _previous_rail {Cyclic_buffer(size)} {
    _id = id;
    _rail_id = track_id;
    _size = size;

    _orientation = 0;
    _current_speed = 0;
    _max_speed = 100;
    
    _progression = 0.f;
    _direction = 1;
    _wagons.push_back(std::make_unique<Locomotive>(Locomotive()));
    for (std::size_t i=0; i<size; ++i)
        _wagons.push_back(std::make_unique<Wagon>(Wagon("test", 0.f)));
}

Train::~Train() {}

void Train::reverse() {
    _direction *= -1;
    this->_previous_rail.reverse();
}

void Train::draw(Layout layout, std::vector<Rail> rails) {
    auto p = _progression;
    auto rail = _rail_id;
    int prev_rail = 0;
    auto direction = _direction;
    for (std::size_t i = 0; i < _wagons.size() - 1; i++) {
        _wagons[i]->draw(layout, rails, rail, p);
        if ((direction == 1 && p >= 0.5f) || (direction == -1 && p <= 0.5f)) {
            p -= direction * 0.5;
        } else {
            /* rail = get_prev_rail(prev_rail++); */
            if ((std::size_t) prev_rail < _previous_rail.get_size()) {
                auto prev = _previous_rail.get_prev_rail(prev_rail++);
                int new_dir = prev.direction;
                rail = prev.rail_id;
                if (direction > 0) {
                    if (direction == new_dir) {
                        p = 0.5 + (p * direction);
                    } else {
                        p = 0.5 - p;
                    }
                } else {
                    if (direction == new_dir) {
                        p = p - 0.5;
                    } else {
                        p = 1.5 - p;
                    }
                }
                direction = new_dir;
            }
        }
    }
    _wagons[_wagons.size() - 1]->draw(layout, rails, rail, p);
}

void Train::next_rail(Grid* grid) {
    if (_previous_rail.get_size() >= _size) {
        prev_rail_s prev_rail_data = _previous_rail.del_last_prev_rail();
        Rail* prev_rail = grid->get_rail(prev_rail_data.rail_id);
        prev_rail->del_on_track_train(_id);
    }
    _previous_rail.add_prev_rail(_rail_id, _direction);
    grid->get_rail(_rail_id)->add_on_track_train(_id);
    std::set<int> neighbor;
    int new_src_dir;
    if (_direction == 1) {
        //  Dans ce cas,  le tain à traversé le dernier rail de la source vers
        // la destination
        neighbor = grid->graph.get_dst_neighbor(_rail_id);
        new_src_dir =
            Hex::opposite_direction(grid->get_rail(_rail_id)->get_dst_neighbor());
    } else {
        assert(_direction == -1);
        //  Dans ce cas, le train à traversé le dernier rail de la destination
        // vers la source
        neighbor = grid->graph.get_src_neighbor(_rail_id);
        new_src_dir =
            Hex::opposite_direction(grid->get_rail(_rail_id)->get_src_neighbor());
    }

    if (neighbor.size() > 0) {
        _rail_id = *(std::next(neighbor.begin(), rand() % neighbor.size()));
        if (new_src_dir == grid->get_rail(_rail_id)->get_src_neighbor()) {
            // Dans ce cas, on vas maintenant aller de la source à la
            // destination
            _progression = 0.f;
            _direction = 1;
        } else {
            assert(new_src_dir == grid->get_rail(_rail_id)->get_dst_neighbor());
            // Dans ce cas, on vas alors aller de la destination vers la source
            _progression = 1.f;
            _direction = -1;
        }
    } else {
        this->reverse();
    }
}

void Train::update(Grid* grid) {
    _progression += 0.01 * _direction;
    if (_progression > 1.f || _progression < 0.f) {
        next_rail(grid);
    }
}

ItineraryTrain::ItineraryTrain(int id, std::vector<int> path, std::size_t size)
    : Train(id, path[0], size), _path{path}, _position{0} {
    assert(path.size() > 0);
}

ItineraryTrain::~ItineraryTrain() {}

void ItineraryTrain::next_rail(Grid* grid) {
    if ((long unsigned)_position < _path.size() - 1) {
        int new_src_dir;
        if (_direction == 1) {
            //  Dans ce cas,  le tain à traversé le dernier rail de la source
            // vers la destination
            new_src_dir = Hex::opposite_direction(
                grid->get_rail(_path[_position])->get_dst_neighbor());
        } else {
            assert(_direction == -1);
            //  Dans ce cas, le train à traversé le dernier rail de la
            // destination vers la source
            new_src_dir = Hex::opposite_direction(
                grid->get_rail(_path[_position])->get_src_neighbor());
        }
        if (new_src_dir == grid->get_rail(_path[_position + 1])->get_src_neighbor()) {
            // Dans ce cas, on vas maintenant aller de la source à la
            // destination
            _progression = 0.f;
            _direction = 1;
        } else {
            assert(new_src_dir ==
                   grid->get_rail(_path[_position + 1])->get_dst_neighbor());
            // Dans ce cas, on vas alors aller de la destination vers la source
            _progression = 1.f;
            _direction = -1;
        }
        _position++;
    } else {
        // We flip the path
        _direction *= -1;
        _position = 0;
        std::reverse(_path.begin(), _path.end());
    }
    _rail_id = _path[_position];
}
