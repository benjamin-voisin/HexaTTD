#pragma once

#include <vector>

class TrackUF {
    private:
        std::vector<int> parent;
        int n_classes;

    public:
        TrackUF();
        int track_number();
        int add();
        void ens_union(int a, int b);
        int find(int a);
};