#include "track_union_find.hpp"

#include <assert.h>

TrackUF::TrackUF() {};

int TrackUF::track_number() {
    return parent.size();
}

int TrackUF::add() {
    int id = parent.size();
    parent.push_back(id);
    ++n_classes;
    return id;
}

int TrackUF::find(int node) {
    assert((long unsigned) node < parent.size());
    if (node == parent[node])
        return node;
    int value = find(parent[node]);
    parent[node] = value;
    return value;
}

void TrackUF::ens_union(int node1, int node2) {
    int classe1 = find(node1);
    int classe2 = find(node2);
    if (classe1 != classe2) {
        parent[classe1] = classe2;
    }
}