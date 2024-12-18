#pragma once

#include <math.h>

#include "geometry/layout.hpp"
#include "rail.hpp"
#include "station.hpp"
#include "tile.hpp"
#include "track_graph/track_graph.hpp"
#include "train/train.hpp"

#define WHEEL_FACTOR 5

class Grid {
  private:
    int q_min;
    int q_max;
    int r_min;
    int r_max;
    std::vector<Tile> tiles;
    std::vector<Rail> rails;
    std::vector<Train *> trains;
    std::vector<Station> stations;

    std::mutex _lock;

    bool _running;

  public:
    Graph graph;
    Layout layout;
    void draw();
    void hightlight(Hex hex, Color c);
    Grid(Orientation orientation, Vector2 size, Vector2 origin, int q_min,
         int q_max, int r_min, int r_max);
    Hex xy_to_hex(float x, float y);

    Tile *tile_from_hex(Hex hex);
    void add_rail(Hex tile, int src_side, int dst_side, int width);
    void add_train(int track_id, std::size_t size);

    Rail *get_rail(int track_id);
    void del_rail(int rail_id);

    void add_station(int rail_id, std::string name);

    bool on_grid(Hex hex);

    void update();
    void update_zoom(int wheel_factor, bool center_on_mouse);

    bool is_running();
    void stop();

    ~Grid();
};
