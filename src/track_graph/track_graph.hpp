#pragma once

#include <vector>

#include "track_union_find.hpp"

class Graph {
    private:
        TrackUF uf;
        std::vector<std::vector<int>> adj_list;


    public:
        Graph();
        int size();
        int add(std::vector<int> neighbor);
        int get_class(int node);
        void to_dot(const char* filename);
        
};