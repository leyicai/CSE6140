/**
 * graph.h
 * Kasimir Gabert
 * Georgia Institute of Technology, Fall 2018
 *
 * Headers for a simple CSR based graph object
 */

#ifndef _H_GRAPH_H
#define _H_GRAPH_H

#include <stdlib.h>
#include <cstdint>
#include <unordered_set>

using vid_t = uint64_t;
using eid_t = uint64_t;

using vdata_t = double;

class Graph {
private:
    /** Keep track of the graph size */
    vid_t n;
    eid_t m;

    /** Store the graph as edges and offsets into edges */
    eid_t *edges;
    eid_t *offsets;

    /** Include an array for vertex data */
    vdata_t *vdata;

public:
    eid_t *get_raw_edges() { return edges; }
    eid_t *get_raw_offsets() { return offsets; }

    void print();

    eid_t *edge_begin(vid_t v);
    eid_t *edge_end(vid_t v);

    void set_vdata(vid_t v, vdata_t d);
    vdata_t* vdata_begin();
    vdata_t* vdata_end();

    vid_t get_n() { return n; }

    size_t degree(vid_t v);
    Graph(vid_t n, eid_t m);
    ~Graph();
};

class VertexSet {
private:
    std::unordered_set<vid_t> _set;

public:
    VertexSet() {};
    ~VertexSet() {};
    void insert(vid_t v);
    bool search(vid_t v);

    void print();
};

#endif /* _H_GRAPH_H */
