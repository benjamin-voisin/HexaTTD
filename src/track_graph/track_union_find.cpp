#include "track_union_find.hpp"

#include <assert.h>

TrackUF::TrackUF() {};

int TrackUF::track_number() {
    return parent.size();
}

int TrackUF::add() {
    int id = parent.size();
    parent.push_back(id);
    rang.push_back(0);
    classes.push_back(n_classes);
    classes_version.push_back(version);
    ++n_classes;
    return id;
}

int TrackUF::n_classe() {
    return n_classes;
}

int TrackUF::classe(int a) {
    int id = find(a);
    if (classes_version[id] < version) {
        classes[id] = n_current_classes++;
        classes_version[id] = version;
    }
    return classes[id];
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
        if (rang[classe1] == rang[classe2]) {
            parent[classe2] = classe1;
            rang[classe1] += 1;
        } else if (rang[classe1] > rang[classe2]) {
            parent[classe2] = classe1;
        } else {
            parent[classe1] = classe2;
        }
        --n_classes;
    }
    n_current_classes = 0;
    ++version;
}