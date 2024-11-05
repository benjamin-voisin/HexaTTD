#pragma once

#include <vector>

#include "track_union_find.hpp"
#include "../rail.hpp"

class Graph {
    private:
        TrackUF uf;
        std::vector<std::vector<int>> adj_list_src;
        std::vector<std::vector<int>> adj_list_dst;


    public:
        Graph();
        int size();
        int add(Rail r, std::vector<Rail> rails, std::vector<int> neighbor_src, std::vector<int> neighbor_dst);
        int get_max_class();
        int get_class(int node);
        void to_dot(const char* filename);

        std::vector<int> get_src_neighbor(int id_rail);
        std::vector<int> get_dst_neighbor(int id_rail);

        
};