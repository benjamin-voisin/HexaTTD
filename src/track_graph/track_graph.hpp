#pragma once

#include <set>
#include <vector>

#include "../rail.hpp"
#include "track_union_find.hpp"

class Graph {
  private:
    TrackUF uf;
    std::vector<std::set<int>> adj_list_src;
    std::vector<std::set<int>> adj_list_dst;
    void _fusion(int rail_id);

  public:
    Graph();
    int size();
    int add(Rail r, std::vector<Rail> &rails, std::set<int> neighbor_src,
            std::set<int> neighbor_dst);
    int get_max_class();
    int get_class(int node);
    void delete_rail(int rail_id);
    void to_dot(const char *filename);

    std::set<int> get_src_neighbor(int id_rail);
    std::set<int> get_dst_neighbor(int id_rail);
};
