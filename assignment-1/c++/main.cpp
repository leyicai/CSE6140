/**
 * main.cpp
 * Kasimir Gabert
 * Georgia Institute of Technology, Fall 2018
 *
 * The main entry point for computing HC centrality
 */

#include "graph.h"
#include "io.h"
#include "hcc.h"

#include <stdio.h>
#include <string>

int main(int argc, char **argv) {
    std::string gr_file_path, out_file_path, hn_file_path;

    if (argc != 4) {
        printf("Usage: %s <input-graph-file> <heist-nodes-file> <output-file>\n", argv[0]);
        return -1;
    }

    gr_file_path = std::string(argv[1]);
    hn_file_path = std::string(argv[2]);
    out_file_path = std::string(argv[3]);

    // Open the input and use it to create the graph
    Graph g = IO::graph_from_gr(gr_file_path);

    // Load the vertex set h
    VertexSet h = IO::vertexset_from_file(hn_file_path);
    
    // Compute the HC centrality for each vertex
    HCC::compute(g, h);
    
    // Write out the HC centrality to a file
    IO::write_vdata_to_file(g, out_file_path);

    return 0;
}
