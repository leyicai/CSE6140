/**
 * io.cpp
 * Kasimir Gabert
 * Georgia Institute of Technology, Fall 2018
 *
 * Helper functions for reading graphs and writing data
 */

#include "graph.h"
#include "io.h"
#include <iostream>
#include <fstream>
#include <exception>
#include <iomanip>
#include <limits>

namespace IO {

Graph
graph_from_gr(const std::string& gr_file) {
    // Open the file and read the number of vertices/edges
    std::fstream in(gr_file, std::ios_base::in);

    vid_t n;
    eid_t m;

    in >> n >> m;

    // Then, create the new graph with the appropriate size
    Graph g = Graph(n, m);
    
    eid_t *edges = g.get_raw_edges();
    eid_t *offsets = g.get_raw_offsets();

    // Read through the file parsing edges, assuming that they are in
    // order
    vid_t cur = n;
    for (eid_t offset = 0; offset < m; ++offset) {
        vid_t src, dst;
        in >> src >> dst;
        while (cur != src) {
            // We need to set the offset appropriately
            if (cur == n) cur = 0;
            else cur++;
            if (cur > src) throw std::length_error("invalid edge");

            offsets[cur] = offset;
        }

        // Add the edge in
        edges[offset] = dst;
    }
    // Finish the final edges
    for (++cur; cur < n; ++cur) offsets[cur] = m;

    return g;
}

VertexSet
vertexset_from_file(const std::string& file) {
    // Open the file and read the number of elements in the set
    std::fstream in(file, std::ios_base::in);
    vid_t k;

    in >> k;

    // Then, loop through and read/insert each into a new VertexSet
    VertexSet vs;

    for (vid_t i = 0; i < k; ++i) {
        vid_t v;
        in >> v;
        vs.insert(v);
    }

    return vs;
}

void
write_vdata_to_file(Graph& g, const std::string& file) {
    std::fstream out(file, std::ios_base::out);

    // Write the number of vertices
    out << g.get_n() << std::endl;

    out << std::setprecision(std::numeric_limits<long double>::digits10 + 1);

    // Write out each vertex's data on individual lines
    for (auto iter = g.vdata_begin(); iter < g.vdata_end(); ++iter) {
        out << *iter << std::endl;
    }
}

};
