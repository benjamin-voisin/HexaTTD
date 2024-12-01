#include "train.hpp"
#include <algorithm>
#include <assert.h>
#include <stdio.h>

Train::Train(int track_id) {
    _rail_id = track_id;
    _orientation = 0;
    _prev_rails = (int *)calloc(3, sizeof(int));
    _prev_rails_index = 0;
    _prev_rails_size = 3;
    _current_speed = 0;
    _max_speed = 100;
    _progression = 0.f;
    _direction = 1;
    _wagons.push_back(std::make_unique<Locomotive>(Locomotive()));
    _wagons.push_back(std::make_unique<Wagon>(Wagon("test", 0.f)));
    _wagons.push_back(std::make_unique<Wagon>(Wagon("test", 0.f)));
    _wagons.push_back(std::make_unique<Wagon>(Wagon("test", 0.f)));
}

Train::~Train() { free(_prev_rails); }

void Train::draw(Layout layout, std::vector<Rail> rails) {
    auto p = _progression;
    auto rail = _rail_id;
    int prev_rail = 0;
    auto direction = _direction;
    for (size_t i = 0; i < _wagons.size() - 1; i++) {
        _wagons[i]->draw(layout, rails, rail, p);
        if ((direction == 1 && p >= 0.5f) || (direction == -1 && p <= 0.5f)) {
            p -= direction * 0.5;
        } else {
            rail = get_prev_rail(prev_rail++);
            int new_dir;
            if (rail < 0) {
                rail = -rail;
                new_dir = -1;
            } else {
                new_dir = 1;
            }
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
    _wagons[_wagons.size() - 1]->draw(layout, rails, rail, p);
}

void Train::add_prev_rail(int rail_id, int direction) {
    _prev_rails_index = (_prev_rails_index + 1) % _prev_rails_size;
    _prev_rails[_prev_rails_index] = rail_id * direction;
}

int Train::get_prev_rail(int n) {
    if ((size_t)n > _prev_rails_index) {
        return _prev_rails[_prev_rails_index + _prev_rails_size - n];
    } else {
        return _prev_rails[_prev_rails_index - n];
    }
}

void Train::next_rail(Graph graph, std::vector<Rail> rails) {
    add_prev_rail(_rail_id, _direction);
    std::set<int> neighbor;
    int new_src_dir;
    if (_direction == 1) {
        //  Dans ce cas,  le tain à traversé le dernier rail de la source vers
        // la destination
        neighbor = graph.get_dst_neighbor(_rail_id);
        new_src_dir =
            Hex::opposite_direction(rails[_rail_id].get_dst_neighbor());
    } else {
        assert(_direction == -1);
        //  Dans ce cas, le train à traversé le dernier rail de la destination
        // vers la source
        neighbor = graph.get_src_neighbor(_rail_id);
        new_src_dir =
            Hex::opposite_direction(rails[_rail_id].get_src_neighbor());
    }

    if (neighbor.size() > 0) {
        _rail_id = *(std::next(neighbor.begin(), rand() % neighbor.size()));
        if (new_src_dir == rails[_rail_id].get_src_neighbor()) {
            // Dans ce cas, on vas maintenant aller de la source à la
            // destination
            _progression = 0.f;
            _direction = 1;
        } else {
            assert(new_src_dir == rails[_rail_id].get_dst_neighbor());
            // Dans ce cas, on vas alors aller de la destination vers la source
            _progression = 1.f;
            _direction = -1;
        }
    } else {
        _direction *= -1;
    }
}

void Train::update(Graph graph, std::vector<Rail> rails) {
    _progression += 0.01 * _direction;
    if (_progression > 1.f || _progression < 0.f) {
        next_rail(graph, rails);
    }
}

ItineraryTrain::ItineraryTrain(std::vector<int> path)
    : Train(path[0]), _path{path}, _position{0} {
    assert(path.size() > 0);
}

ItineraryTrain::~ItineraryTrain() {}

void ItineraryTrain::next_rail(__attribute__((unused)) Graph graph,
                               std::vector<Rail> rails) {
    if ((long unsigned)_position < _path.size() - 1) {
        int new_src_dir;
        if (_direction == 1) {
            //  Dans ce cas,  le tain à traversé le dernier rail de la source
            // vers la destination
            new_src_dir = Hex::opposite_direction(
                rails[_path[_position]].get_dst_neighbor());
        } else {
            assert(_direction == -1);
            //  Dans ce cas, le train à traversé le dernier rail de la
            // destination vers la source
            new_src_dir = Hex::opposite_direction(
                rails[_path[_position]].get_src_neighbor());
        }
        if (new_src_dir == rails[_path[_position + 1]].get_src_neighbor()) {
            // Dans ce cas, on vas maintenant aller de la source à la
            // destination
            _progression = 0.f;
            _direction = 1;
        } else {
            assert(new_src_dir ==
                   rails[_path[_position + 1]].get_dst_neighbor());
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
