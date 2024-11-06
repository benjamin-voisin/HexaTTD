#include "track_union_find.hpp"

#include <assert.h>

TrackUF::TrackUF() {
	n_current_classes = 0;
	n_classes = 0;
	version = 0;
};

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

std::vector<int> TrackUF::get_class(int a) {
    int classe_a = find(a);
    std::vector<int> member_classe_a = {};
    for (auto i=0; i<parent.size(); ++i)
        if ((parent[i] != -1) && (find(parent[i]) == classe_a))
            member_classe_a.push_back(i);
    return member_classe_a;
}

std::vector<int> TrackUF::del(int a) {
    int classe_a = find(a);
    std::vector<int> member_classe_a = get_class(a);
    for (auto i=0; i<member_classe_a.size(); ++i)
        parent[member_classe_a[i]] = member_classe_a[i];
    parent[a] = -1;
    version += 1;
    // Not adding a real class but a possible one
    n_classes += 1;
    return member_classe_a;
}

int TrackUF::find(int node) {
    assert(node != -1);
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
