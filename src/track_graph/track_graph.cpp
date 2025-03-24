#include "track_graph.hpp"
#include "../log.hpp"

#include <assert.h>
#include <stdio.h>

Graph::Graph() {};

int Graph::size() { return this->adj_list_src.size(); }

std::set<int> Graph::get_src_neighbor(int id_rail) {
    assert((long unsigned)id_rail < adj_list_src.size());
    assert(0 <= id_rail);
    return adj_list_src[id_rail];
}

std::set<int> Graph::get_dst_neighbor(int id_rail) {
    assert((long unsigned)id_rail < adj_list_dst.size());
    assert(0 <= id_rail);
    return adj_list_dst[id_rail];
}

void Graph::_fusion(int rail_id) {
    assert((0 <= rail_id) && (long unsigned)rail_id < adj_list_src.size());

    std::set<int> neighbor_src = adj_list_src[rail_id];
    std::set<int> neighbor_dst = adj_list_dst[rail_id];

    for (auto n = neighbor_src.begin(); n != neighbor_src.end(); ++n) {
        assert((long unsigned)*n < adj_list_src.size());
        uf.ens_union(*n, rail_id);
    }
    for (auto n = neighbor_dst.begin(); n != neighbor_dst.end(); ++n) {
        assert((long unsigned)*n < adj_list_dst.size());
        uf.ens_union(*n, rail_id);
    }
}

int Graph::add(Rail r, std::vector<Rail> &rails, std::set<int> neighbor_src,
               std::set<int> neighbor_dst) {
    adj_list_src.push_back(neighbor_src);
    adj_list_dst.push_back(neighbor_dst);
    int id = uf.add();
    for (auto n = neighbor_src.begin(); n != neighbor_src.end(); ++n) {
        assert((long unsigned)*n < adj_list_src.size());
        if (Hex::opposite_direction(r.get_src_neighbor()) ==
            rails[*n].get_src_neighbor()) {
            adj_list_src[*n].insert(id);
        } else {
#ifndef NDEBUG
            Log::Info.log("rsrc=%d, nsrc=%d ndst=%d", r.get_src_neighbor(),
                          rails[*n].get_src_neighbor(),
                          rails[*n].get_dst_neighbor());
#endif // !NDEBUG
            assert(Hex::opposite_direction(r.get_src_neighbor()) ==
                   rails[*n].get_dst_neighbor());
            adj_list_dst[*n].insert(id);
        }
    }
    for (auto n = neighbor_dst.begin(); n != neighbor_dst.end(); ++n) {
        assert((long unsigned)*n < adj_list_dst.size());
        if (r.get_dst_neighbor() ==
            Hex::opposite_direction(rails[*n].get_src_neighbor())) {
            adj_list_src[*n].insert(id);
        } else {
            assert(r.get_dst_neighbor() ==
                   Hex::opposite_direction(rails[*n].get_dst_neighbor()));
            adj_list_dst[*n].insert(id);
        }
    }
    _fusion(id);
    return id;
}

void Graph::delete_rail(int rail_id) {
    assert((0 <= rail_id) && (long unsigned)rail_id < adj_list_src.size());

    std::set<int> neighbor_src = adj_list_src[rail_id];
    for (auto n = neighbor_src.begin(); n != neighbor_src.end(); ++n) {
        adj_list_src[*n].erase(rail_id);
        adj_list_dst[*n].erase(rail_id);
    }
    std::set<int> neighbor_dst = adj_list_dst[rail_id];
    for (auto n = neighbor_dst.begin(); n != neighbor_dst.end(); ++n) {
        adj_list_src[*n].erase(rail_id);
        adj_list_dst[*n].erase(rail_id);
    }
    adj_list_src[rail_id] = {};
    adj_list_dst[rail_id] = {};

    std::vector<int> member_classe = uf.del(rail_id);
    for (auto n = member_classe.begin(); n != member_classe.end(); ++n) {
        _fusion(*n);
    }
}

int Graph::get_class(int node) { return uf.classe(node); }

int Graph::get_max_class() { return uf.n_classe(); }

#include <stdio.h>
#include <stdlib.h>
void Graph::to_dot(const char *filename) {
    FILE *f = fopen(filename, "w");
    fprintf(f, "digraph {\n");
    fprintf(f, "layout=fdp\n");
    for (long unsigned i = 0; i < adj_list_src.size(); ++i) {
        fprintf(f, "// noeud %ld\n", i);
        for (auto n = adj_list_src[i].begin(); n != adj_list_src[i].end();
             ++n) {
            fprintf(f, "   src%ld -> %d;\n", i, *n);
        }
        for (auto n = adj_list_src[i].begin(); n != adj_list_src[i].end();
             ++n) {
            fprintf(f, "   dst%ld -> %d;\n", i, *n);
        }
        fprintf(f, "    %ld -> src%ld;\n", i, i);
        fprintf(f, "    %ld -> dst%ld;\n", i, i);
    }
    fprintf(f, "}\n");
    fclose(f);
}
