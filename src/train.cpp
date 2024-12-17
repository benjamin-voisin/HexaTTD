#include "train.hpp"
#include "grid.hpp"

#include <algorithm>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

Train::Train(Grid *grid, int id, int track_id, std::size_t size)
    : _previous_rail{Cyclic_buffer(size / 2 + 2)} {
    _id = id;

    _size = size;

    _progression = 0.f;

    _previous_rail.add_prev_rail(track_id, 1);
    grid->get_rail(track_id)->add_on_track_train(_id);

    _wagons.push_back(std::make_unique<Locomotive>(Locomotive()));
    for (std::size_t i = 0; i < size - 1; ++i)
        _wagons.push_back(std::make_unique<Wagon>(Wagon("test", 0.f)));
}

Train::~Train() {}

void Train::reverse() {
    // Reverse the Cyclic buffer
    this->_previous_rail.reverse();
    // Reverse the order of the wagons
    std::reverse(_wagons.begin(), _wagons.end());
    // If we have an even number of wagons, we nee to change the progression
    // value
    if (_size % 2 == 0)
        _progression -= 0.5;
}

void Train::draw(Layout layout, std::vector<Rail> rails) {
    std::size_t c = (_progression > 0.5) ? 0 : 1;
    for (std::size_t i = 0; i < _wagons.size(); i++) {
        auto j = (i + c + 1) / 2;
        if (j < _previous_rail.get_size()) {
            auto prev = _previous_rail.get_prev_rail(j);
            auto p = _progression;
            // Gestion de l'autre demi-rail
            // décalage par rapport à la locomotive
            if (i % 2 != 1) {
                if (c == 0)
                    p -= 0.5;
                else
                    p += 0.5;
            }
            // Direction du rail opposée
            // Il faut inverser la progression courante
            if (prev.direction == -1)
                p = 1 - p;
            _wagons[i]->draw(layout, rails, prev.rail_id, p);
        }
    }
}

void Train::next_rail(Grid *grid) {
    prev_rail_s prev_rail = _previous_rail.get_prev_rail(0);
    auto _direction = prev_rail.direction;
    auto _rail_id = prev_rail.rail_id;

    std::set<int> neighbor;
    int new_src_dir;

    if (_previous_rail.get_size() >= _previous_rail.get_max_size()) {
        prev_rail_s prev_rail_data = _previous_rail.del_last_prev_rail();
        Rail *prev_rail = grid->get_rail(prev_rail_data.rail_id);
        prev_rail->del_on_track_train(_id);
    }

    if (_direction == 1) {
        //  Dans ce cas,  le tain à traversé le dernier rail de la source vers
        // la destination
        neighbor = grid->graph.get_dst_neighbor(_rail_id);
        new_src_dir = Hex::opposite_direction(
            grid->get_rail(_rail_id)->get_dst_neighbor());
    } else {
        assert(_direction == -1);
        //  Dans ce cas, le train à traversé le dernier rail de la destination
        // vers la source
        neighbor = grid->graph.get_src_neighbor(_rail_id);
        new_src_dir = Hex::opposite_direction(
            grid->get_rail(_rail_id)->get_src_neighbor());
    }

    if (neighbor.size() > 0) {
        _rail_id = *(std::next(neighbor.begin(), rand() % neighbor.size()));
        if (new_src_dir == grid->get_rail(_rail_id)->get_src_neighbor()) {
            // Dans ce cas, on vas maintenant aller de la source à la
            // destination
            _direction = 1;
        } else {
            assert(new_src_dir == grid->get_rail(_rail_id)->get_dst_neighbor());
            // Dans ce cas, on vas alors aller de la destination vers la source
            _direction = -1;
        }
        _progression = 0.f;
        _previous_rail.add_prev_rail(_rail_id, _direction);
        grid->get_rail(_rail_id)->add_on_track_train(_id);
    } else {
        this->reverse();
    }
}

void Train::update(Grid *grid) {

    _progression += 0.01;
    if (_progression > 1.f)
        next_rail(grid);
}

ItineraryTrain::ItineraryTrain(Grid *grid, int id, std::vector<int> path,
                               std::size_t size)
    : Train(grid, id, path[0], size), _path{path}, _position{0} {
    assert(path.size() > 0);
}

ItineraryTrain::~ItineraryTrain() {}

void ItineraryTrain::next_rail(Grid *grid) {
    prev_rail_s prev_rail = _previous_rail.get_prev_rail(0);
    if ((long unsigned)_position < _path.size() - 1) {
        int new_src_dir;
        if (prev_rail.direction == 1) {
            //  Dans ce cas,  le tain à traversé le dernier rail de la source
            // vers la destination
            new_src_dir = Hex::opposite_direction(
                grid->get_rail(_path[_position])->get_dst_neighbor());
        } else {
            assert(prev_rail.direction == -1);
            //  Dans ce cas, le train à traversé le dernier rail de la
            // destination vers la source
            new_src_dir = Hex::opposite_direction(
                grid->get_rail(_path[_position])->get_src_neighbor());
        }
        if (new_src_dir ==
            grid->get_rail(_path[_position + 1])->get_src_neighbor()) {
            // Dans ce cas, on vas maintenant aller de la source à la
            // destination
            _progression = 0.f;
            prev_rail.direction = 1;
        } else {
            assert(new_src_dir ==
                   grid->get_rail(_path[_position + 1])->get_dst_neighbor());
            // Dans ce cas, on vas alors aller de la destination vers la source
            _progression = 1.f;
            prev_rail.direction = -1;
        }
        _position++;
    } else {
        // We flip the path
        _position = 0;
        std::reverse(_path.begin(), _path.end());
    }
    //_rail_id = _path[_position];
}
