#include "track_graph.hpp"

#include <assert.h>

Graph::Graph() {};

int Graph::size() {
    return this->adj_list.size();
}

int Graph::add(std::vector<int> neighbor) {
    adj_list.push_back(neighbor);
    int id = uf.add();
    for (auto n = neighbor.begin(); n != neighbor.end(); ++n) {
        assert((long unsigned) *n < adj_list.size());
        uf.ens_union(*n, id);
        adj_list[*n].push_back(id);
    }
    return id; 
}

int Graph::get_class(int node) {
    return uf.classe(node);
}

int Graph::get_max_class() {
    return uf.n_classe();
}

#include <stdio.h>
#include <stdlib.h>
void Graph::to_dot(const char* filename) {
    FILE* f = fopen(filename, "w");
    fprintf(f, "graph {\n");
    fprintf(f, "layout=fdp\n");
    for (long unsigned i=0; i<adj_list.size(); ++i) {
        fprintf(f, "// noeud %ld\n", i);
        for (long unsigned n=0; n<adj_list[i].size(); ++n) {
            fprintf(f, "   %ld -- %d;\n", i, adj_list[i][n]);
        }
    }
    fprintf(f, "}\n");
    fclose(f);
}