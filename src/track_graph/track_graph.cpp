#include "track_graph.hpp"

#include <assert.h>
#include <stdio.h>

Graph::Graph() {};

int Graph::size() {
    return this->adj_list_src.size();
}

std::vector<int> Graph::get_src_neighbor(int id_rail) {
    assert((long unsigned) id_rail < adj_list_src.size());
    assert(0 <= id_rail);
    return adj_list_src[id_rail];
}

std::vector<int> Graph::get_dst_neighbor(int id_rail) {
    assert((long unsigned) id_rail < adj_list_dst.size());
    assert(0 <= id_rail);
    return adj_list_dst[id_rail];
}

int Graph::add(Rail r, std::vector<Rail> rails, std::vector<int> neighbor_src, std::vector<int> neighbor_dst) {
    adj_list_src.push_back(neighbor_src);
    adj_list_dst.push_back(neighbor_dst);  
    int id = uf.add();
    for (auto n = neighbor_src.begin(); n != neighbor_src.end(); ++n) {
        assert((long unsigned) *n < adj_list_src.size());
        uf.ens_union(*n, id);
        if (Hex::opposite_direction(r.get_src_neighbor()) == rails[*n].get_src_neighbor()) {
            adj_list_src[*n].push_back(id);
        } else {
            printf("rsrc=%d, nsrc=%d ndst=%d\n", r.get_src_neighbor(), rails[*n].get_src_neighbor(), rails[*n].get_dst_neighbor());
            assert(Hex::opposite_direction(r.get_src_neighbor()) == rails[*n].get_dst_neighbor());
            adj_list_dst[*n].push_back(id);
        }
        
    }
    for (auto n = neighbor_dst.begin(); n != neighbor_dst.end(); ++n) {
        assert((long unsigned) *n < adj_list_dst.size());
        uf.ens_union(*n, id);
        if (r.get_dst_neighbor() == Hex::opposite_direction(rails[*n].get_src_neighbor())) {
            adj_list_src[*n].push_back(id);
        } else {
            assert(r.get_dst_neighbor() == Hex::opposite_direction(rails[*n].get_dst_neighbor()));
            adj_list_dst[*n].push_back(id);
        }
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
    fprintf(f, "digraph {\n");
    fprintf(f, "layout=fdp\n");
    for (long unsigned i=0; i<adj_list_src.size(); ++i) {
        fprintf(f, "// noeud %ld\n", i);
        for (long unsigned n=0; n<adj_list_src[i].size(); ++n) {
            fprintf(f, "   src%ld -> %d;\n", i, adj_list_src[i][n]);
        }
        for (long unsigned n=0; n<adj_list_dst[i].size(); ++n) {
            fprintf(f, "   dst%ld -> %d;\n", i, adj_list_dst[i][n]);
        }
        fprintf(f, "    %ld -> src%ld;\n", i, i);
        fprintf(f, "    %ld -> dst%ld;\n", i, i);
    }
    fprintf(f, "}\n");
    fclose(f);
}