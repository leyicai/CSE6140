/**
 * graph.cpp
 * Kasimir Gabert
 * Georgia Institute of Technology, Fall 2018
 *
 * A simple CSR based graph object
 */

#include "graph.h"

#include <iostream>

size_t
Graph::degree(vid_t v) {
    // Degree is either: the different between the next element's pointer,
    // or the difference between the last element and the total number of
    // edges
    if (v == n-1)
        return m-offsets[v];
    else
        return offsets[v+1]-offsets[v];
}

Graph::Graph(vid_t n, eid_t m) : n(n), m(m) {
    // Allocate memory for the offsets and edges
    edges = (eid_t*)calloc(sizeof(eid_t), m);
    offsets = (eid_t*)calloc(sizeof(eid_t), n);

    // Allocate vertex data
    vdata = (vdata_t*)calloc(sizeof(vdata_t), n);
}

Graph::~Graph() {
    free(edges);
    free(offsets);
    free(vdata);
}

void
Graph::set_vdata(vid_t v, vdata_t d) {
    vdata[v] = d;
}

vdata_t*
Graph::vdata_begin() {
    return &vdata[0];
}

vdata_t*
Graph::vdata_end() {
    return &vdata[n];
}

eid_t*
Graph::edge_begin(vid_t v) {
    return &edges[offsets[v]];
}

eid_t*
Graph::edge_end(vid_t v) {
    return &edges[offsets[v]+degree(v)];
}

void
Graph::print() {
    std::cout << "Graph: n=" << n << " m=" << m << std::endl;
    std::cout << "Offsets: ";
    for (vid_t i = 0; i < n; ++i)
        std::cout << offsets[i] << " ";
    std::cout << std::endl;
    std::cout << "Edges: ";
    for (eid_t i = 0; i < m; ++i)
        std::cout << edges[i] << " ";
    std::cout << std::endl;

    std::cout << "Structure: " << std::endl;
    for (vid_t v = 0; v < n; v++) {
        std::cout << v << "->";
        for (eid_t *e = edge_begin(v); e < edge_end(v); ++e)
            std::cout << *e << " ";
        std::cout << std::endl;
    }
}

void
VertexSet::insert(vid_t v) {
    _set.insert(v);
}

bool
VertexSet::search(vid_t v) {
    return (_set.find(v) != _set.end());
}

void
VertexSet::print() {
    std::cout << "VertexSet: k=" << _set.size() << std::endl;
    for (auto iter = _set.begin(); iter != _set.end(); ++iter) {
        std::cout << *iter << " ";
    }
    std::cout << std::endl;
}
